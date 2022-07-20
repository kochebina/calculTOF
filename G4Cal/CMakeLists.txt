cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(Simulation)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

file(GLOB macros ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(sim mysimu.cc ${sources} ${headers})
target_link_libraries(sim ${Geant4_LIBRARIES})


file(GLOB MACRO_FILES 
	"*.mac"
)

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/macros)

file(GLOB DATA_FILES 
	"*.dat"
)

file(COPY ${DATA_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/datas)



add_custom_target(Simulation DEPENDS sim)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS sim DESTINATION bin)
