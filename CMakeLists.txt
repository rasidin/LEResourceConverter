cmake_minimum_required(VERSION 3.2)

project(LEResourceConverter)
enable_testing()

include(zlib.cmake)
include(openexr.cmake)

add_subdirectory(LimitEngineV2)
add_subdirectory(LEMath)
add_subdirectory(CUDAImageUtilities)

add_definitions(-DWINDOWS)

file(GLOB_RECURSE FILES_SOURCE
	"Source/*.cpp"
)
source_group("Source" FILES ${FILES_SOURCE})
file(GLOB_RECURSE FILES_HEADER
	"Source/*.h"
)
source_group("Header" FILES ${FILES_HEADER})

add_executable(LEResourceConverter ${FILES_SOURCE})

target_link_libraries(LEResourceConverter
	LimitEngine 
	LEMath
	CUDAImageUtilities
	zlibd
	IlmImf-2_4_d
	d3d12 dxgi d3dcompiler dxguid
)

add_test(TestConvertEXR LEResourceConverter let -i=../../Test/TestLET.exr -o=../../Test/TestLET.lea)
add_test(TestGenerateIrradianceMap LEResourceConverter irr -i=../../Test/TestIRR.exr -w=2048 -h=1024 -s=1024 -o=../../Test/TestIRR.lea)

install(TARGETS LEResourceConverter DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)