cmake_minimum_required(VERSION 3.2)

project(LEResourceConverter)
enable_testing()

include(LEMath.cmake)
include(LimitEngine.cmake)
include(zlib.cmake)
include(openexr.cmake)

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
	zlibd
	IlmImf-2_4_d
	d3d12 dxgi d3dcompiler dxguid
)

add_test(TestConvertEXR LEResourceConverter let -i=../../Test/MyCarlots.Background.2K.exr -o=../../Test/MyCarlots.Background.2K.lea)

install(TARGETS LEResourceConverter DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)