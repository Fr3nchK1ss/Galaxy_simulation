#include "Computer.hpp"
#include "Simulator.hpp"

size_t                          Computer::simulation_iterations;
float                           Computer::simulation_time;
std::vector<float>              Computer::masses;
std::vector<dim::Vector4>	Computer::positions;
std::vector<dim::Vector4>	Computer::speeds;
std::vector<dim::Vector4>	Computer::accelerations;
cl::Buffer                      Computer::masses_buffer;
cl::Buffer					Computer::positions_buffer;
cl::Buffer					Computer::speeds_buffer;
cl::Buffer					Computer::accelerations_buffer;
cl::Buffer					Computer::step_buffer;
cl::Buffer					Computer::smoothing_length_buffer;
cl::Buffer					Computer::interaction_rate_buffer;
cl::Buffer					Computer::black_hole_mass_buffer;


void Computer::random_sphere(dim::Vector4& position, float& mass)
{
	dim::Vector3 result = dim::Vector3::null;

	do
	{
		result.x = dim::random_float(-Simulator::galaxy_diameter / 2.f, Simulator::galaxy_diameter / 2.f);
		result.y = dim::random_float(-Simulator::galaxy_diameter / 2.f, Simulator::galaxy_diameter / 2.f);
		result.z = dim::random_float(-Simulator::galaxy_diameter / 2.f, Simulator::galaxy_diameter / 2.f);
	}
	while (result.get_norm() > Simulator::galaxy_diameter / 2.f);
    position = dim::Vector4(result, 0.0f);
    mass = 1.0f;
}


void Computer::random_sphere(const float negative_mass_proportion, dim::Vector4& position, float& mass)
{
    dim::Vector3 result = dim::Vector3::null;
    do
    {
        result.x = dim::random_float(-Simulator::galaxy_diameter / 2.f, Simulator::galaxy_diameter / 2.f);
        result.y = dim::random_float(-Simulator::galaxy_diameter / 2.f, Simulator::galaxy_diameter / 2.f);
        result.z = dim::random_float(-Simulator::galaxy_diameter / 2.f, Simulator::galaxy_diameter / 2.f);
    }
    while (result.get_norm() > Simulator::galaxy_diameter / 2.f);
    position = dim::Vector4(result, 0.0f);
    mass = (dim::random_float(0.0f, 1.0f) < negative_mass_proportion) ? -1.0f : 1.0f;
}

void Computer::create_galaxy(int i)
{
	positions[i].set_norm(static_cast<float>(pow(positions[i].get_norm() / (Simulator::galaxy_diameter / 2.f), 5)) * (Simulator::galaxy_diameter / 2.f));
	positions[i].y *= Simulator::galaxy_thickness / Simulator::galaxy_diameter;
	speeds[i] = dim::Vector4(dim::normalize(dim::Vector3(positions[i] + dim::Vector4(0.0000001f, 0.0000001f, 0.0000001f, 1.0f)) ^ dim::Vector3(0.f, 1.f, 0.f)) * Simulator::stars_speed, 0.f);
}

void Computer::create_collision(int i)
{
	create_galaxy(i);

	if (i % 2)
	{
		positions[i].x -= Simulator::galaxies_distance / 2.f;
	}
	else
	{
		positions[i].x += Simulator::galaxies_distance / 2.f;
		std::swap(positions[i].y, positions[i].z);
		std::swap(speeds[i].y, speeds[i].z);
	}

}

void Computer::create_universe(int i)
{
	speeds[i] = positions[i];
	speeds[i].set_norm((speeds[i].get_norm() / (Simulator::galaxy_diameter / 2.f)) * Simulator::stars_speed);
}

void Computer::init()
{
	masses.clear();
	positions.clear();
	speeds.clear();
	accelerations.clear();

	masses.resize(Simulator::nb_stars);
	positions.resize(Simulator::nb_stars);
	speeds.resize(Simulator::nb_stars);
	accelerations.resize(Simulator::nb_stars, dim::Vector4::null);

	for (int i = 0; i < Simulator::nb_stars; i++)
	{
        if (Simulator::simulation_model == SimulationModel::Old)
        {
            // All masses are +1
            random_sphere(positions[i], masses[i]);
        }
        else // Newton, anti-newton
        {
            // Masses are randomized +1 or -1
            random_sphere(Simulator::negative_mass_proportion, positions[i], masses[i]);
        }

		switch (Simulator::simulation_type)
		{
		case SimulationType::Galaxy: create_galaxy(i); break;
		case SimulationType::Collision: create_collision(i); break;
		case SimulationType::Universe: create_universe(i); break;
		default: break;
		}
	}


    masses_buffer = ComputeShader::Buffer(masses, Permissions::All);
	positions_buffer = ComputeShader::Buffer(positions, Permissions::All);
	speeds_buffer = ComputeShader::Buffer(speeds, Permissions::All);
	accelerations_buffer = ComputeShader::Buffer(accelerations, Permissions::All);
	step_buffer = ComputeShader::Buffer(Simulator::step, Permissions::Read);
	smoothing_length_buffer = ComputeShader::Buffer(Simulator::smoothing_length, Permissions::Read);
	interaction_rate_buffer = ComputeShader::Buffer(Simulator::interaction_rate, Permissions::Read);
	black_hole_mass_buffer = ComputeShader::Buffer(Simulator::black_hole_mass, Permissions::Read);

    simulation_iterations = 0;
    simulation_time = 0.0f;
}

void Computer::compute()
{
	step_buffer = ComputeShader::Buffer(Simulator::step, Permissions::Read);
	smoothing_length_buffer = ComputeShader::Buffer(Simulator::smoothing_length, Permissions::Read);
	interaction_rate_buffer = ComputeShader::Buffer(Simulator::interaction_rate, Permissions::Read);
	black_hole_mass_buffer = ComputeShader::Buffer(Simulator::black_hole_mass, Permissions::Read);

	// The interactions computations.
	switch (Simulator::simulation_model)
	{
	case SimulationModel::Old:
		ComputeShader::launch("interactions_old", {
			&positions_buffer, &accelerations_buffer, &interaction_rate_buffer,
			&smoothing_length_buffer, &black_hole_mass_buffer
		}, cl::NDRange(accelerations.size()));
	break;
	case SimulationModel::Newton:
		ComputeShader::launch("interactions_newton", {
			&masses_buffer, &positions_buffer, &accelerations_buffer, &interaction_rate_buffer,
			&smoothing_length_buffer
		}, cl::NDRange(accelerations.size()));
	break;
	case SimulationModel::AntiNewton:
		ComputeShader::launch("interactions_anti_newton", {
			&masses_buffer, &positions_buffer, &accelerations_buffer, &interaction_rate_buffer,
			&smoothing_length_buffer
		}, cl::NDRange(accelerations.size()));
	break;
	default: break;
	}
	ComputeShader::get_data(accelerations_buffer, accelerations);

	// The integration computation.
	ComputeShader::launch("integration", { &positions_buffer, &speeds_buffer, &accelerations_buffer, &step_buffer }, cl::NDRange(speeds.size()));
	ComputeShader::get_data(positions_buffer, positions);
	ComputeShader::get_data(speeds_buffer, speeds);
	
	simulation_iterations++;
    simulation_time += Menu::step;
}
