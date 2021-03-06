# Find the manual here:
# http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/InstallationGuide/html/ch03s02.html

# check for cmake version, initialize project -> sets name and paths
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(mdom)

# choose option
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)

# locate geant4 and set all required variables
if(WITH_GEANT4_UIVIS)
	find_package(Geant4 REQUIRED ui_all vis_all)
else()
	find_package(Geant4 REQUIRED)
endif()

# try to use ROOT libraries
#set (CMAKE_MODULE_PATH /Applic.ZIV/CentOS7/software/p0kapp/software/geant4/10.4.02.p01/lib64/Geant4-10.2.1/Modules/)
set (CMAKE_MODULE_PATH /Applic.ZIV/CentOS7/software/p0kapp/software/geant4/4.09.06.p04/lib64/Geant4-9.6.4/Modules)

#list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
#find_package(ROOT)

# sets search paths for include
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${ROOT_INCLUDE_DIR})
LINK_DIRECTORIES(/Applic.ZIV/CentOS7/software/p0kapp/software/lib)

# create list of all the files to compile
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

# details for the finished binary
add_executable(mdom_K40 mdom_K40.cc ${sources} ${headers})
target_link_libraries(mdom_K40 ${Geant4_LIBRARIES} argtable2)

#add_executable(mdom_scan_angular mdom_scan_angular.cc ${sources} ${headers})
#target_link_libraries(mdom_scan_angular ${Geant4_LIBRARIES} argtable2)

#add_executable(mdom_scan_xyz mdom_scan_xyz.cc ${sources} ${headers})
#target_link_libraries(mdom_scan_xyz ${Geant4_LIBRARIES} argtable2)


# which auxiliary files out of the source directory should be copied to the directory of the binary
set(mdom_aux
	aux/vis.mac
	aux/vis.ogl
	#DecayGuns/K40.gps
	mdom_DecayMenu.gui
	#mdom_scan_angular.gps
	#mdom_scan_xyz.gps
	#QuantumEfficiency.cfg
)

foreach(_script ${mdom_aux})
	configure_file(
		${PROJECT_SOURCE_DIR}/${_script}
		${PROJECT_BINARY_DIR}/${_script}
	COPYONLY
	)
endforeach()
