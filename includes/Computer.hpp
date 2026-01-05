#ifndef COMPUTATION_HPP
#define COMPUTATION_HPP

#include "libraries.hpp"

/**
 * @brief Takes care of the simulation calculations.
 */
class Computer
{
public:

	static size_t                       simulation_iterations;      // Count the number of iterations.
    static float                        simulation_time;            //
	static std::vector<dim::Vector4>    colors;                         // The color of the stars.
    static std::vector<float>           masses;                     // The mass of the stars.
	static std::vector<dim::Vector4>	positions;					// The position of the stars.
	static std::vector<dim::Vector4>	speeds;						// The speed of the stars.
	static std::vector<dim::Vector4>	accelerations;				// The acceleration of the stars.
	static cl::Buffer                   masses_buffer;                  // The OpenCL buffer of the masses.
	static cl::Buffer					positions_buffer;			// The OpenCL buffer of the positions.
	static cl::Buffer					speeds_buffer;				// The OpenCL buffer of the speeds.
	static cl::Buffer					accelerations_buffer;		// The OpenCL buffer of the accelerations.
	static cl::Buffer					step_buffer;				// The OpenCL buffer of the time step.
	static cl::Buffer					smoothing_length_buffer;	// The OpenCL buffer of the smoothing length.
	static cl::Buffer					interaction_rate_buffer;	// The OpenCL buffer of the interaction rate.
	static cl::Buffer					black_hole_mass_buffer;		// The OpenCL buffer of the mass of the black hole.

	/**
	 * @brief Gives a point randomly placed in a sphere of the diameter of the galaxy.
	 *
	 * @return the randomly placed point with fixed mass of 1
	 */
    static void random_sphere(dim::Vector4& position, float& mass);


    /**
     * @brief Gives a point randomly placed in a sphere of the diameter of the galaxy.
     *
     * @return the randomly placed point with randomize positive or negative mass.
     */
    static void random_sphere(const float negative_mass_proportion, dim::Vector4& position, float& mass);
	
	/**
	 * @brief Add a star in the galaxy setup.
	 *
	 * @param i the index of the star
	 */
	static void create_galaxy(int i);

	/**
	 * @brief Add a star in the collision setup.
	 *
	 * @param i the index of the star
	 */
	static void create_collision(int i);

	/**
	 * @brief Add a star in the universe setup.
	 *
	 * @param i the index of the galaxy
	 */
	static void create_universe(int i);

	/**
	 * @brief Initialize the computer.
	 */
	static void init();

	/**
	 * @brief Does the simulation computations of the current frame.
	 */
	static void compute();
};

#endif
