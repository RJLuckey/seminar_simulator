# cmake_minimum_required(VERSION <specify CMake version here>)
project(evo)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)
find_package( OpenCV REQUIRED core highgui imgproc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(evo main.cpp World.cpp header/World.h header/Sensor.h header/Controllable.h header/Placeable.h LightSource.cpp header/LightSource.h Collidable.cpp header/Collidable.h Wall.cpp header/Wall.h Vehicle.cpp header/Vehicle.h LightVehicle.cpp header/LightVehicle.h LightSensor.cpp header/LightSensor.h Placeable.cpp Sensor.cpp Draw.cpp header/Draw.h ProxSensor.cpp header/ProxSensor.h ProxVehicle.cpp header/ProxVehicle.h Genome.cpp header/Genome.h
        network/Ann/Ann.cpp network/Ann/Ann.h network/Layer/Layer.cpp network/Layer/Layer.h network/Connection/Connection.cpp network/Connection/Connection.h network/Neuron/Neuron.cpp network/Neuron/Neuron.h
        constants.h ChargeVehicle.cpp header/ChargeVehicle.h GroundSensor.cpp header/GroundSensor.h Battery.cpp header/Battery.h BatSensor.cpp header/BatSensor.h VehicleSensor.cpp header/VehicleSensor.h header/ExplorerVehicle.h ExplorerVehicle.cpp header/types.h header/plotter.h Mboa.cpp header/Mboa.h ElitesMap.cpp header/ElitesMap.h Plotter.cpp)
add_subdirectory(matplotplusplus)

target_link_libraries(evo PUBLIC matplot)
target_link_libraries(evo PRIVATE ${OpenCV_LIBS})


include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/${PACKAGE_NAME}/cmake)





