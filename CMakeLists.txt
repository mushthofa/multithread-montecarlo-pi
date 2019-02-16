cmake_minimum_required(VERSION 2.6)
project(pimc)


add_executable(pimc main.cpp)

target_link_libraries(pimc boost_program_options)
target_link_libraries(pimc boost_thread)
target_link_libraries(pimc boost_system)

install(TARGETS pimc RUNTIME DESTINATION bin)
