# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 2.6)
project(qxcfg
        VERSION 0.1
        DESCRIPTION "Quick XML handling and XML-based configuration")

find_package(Rock)
rock_init()
rock_activate_cxx11()
rock_standard_layout()
