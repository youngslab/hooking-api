


add_library(hook INTERFACE)

target_link_libraries(hook INTERFACE dl)

target_include_directories(hook INTERFACE ./include)
