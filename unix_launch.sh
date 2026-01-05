/usr/bin/cmake -G "Unix Makefiles" -B "./build" .
cd build

if [ "$(uname -s)" == "Linux" ]; then


  make -j`nproc --all`


elif [ "$(uname -s)" == "Darwin" ]; then


  make -j`sysctl -n hw.logicalcpu`


else


  make -j8


fi


RES=`echo $?`
cd ..
