rock_library(qxcfg
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(qxcfg_bin Main.cpp
    DEPS qxcfg)

