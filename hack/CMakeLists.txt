

add_library(hack SHARED hacked.cpp)

target_include_directories(hack PRIVATE ../lib)

target_link_libraries(hack dl)

set_target_properties(hack PROPERTIES
		CXX_STANDARD 17)
