# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 2.6)
find_package(Rock)
rock_init(qxcfg 0.1)
rock_activate_cxx11()
set(PROJECT_DESCRIPTION "Quick XML handling and XML-based configuration")
rock_standard_layout()
