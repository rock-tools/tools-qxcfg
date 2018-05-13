find_package(Boost REQUIRED filesystem system)
include_directories(${Boost_INCLUDE_DIR})

rock_library(qxcfg
    SOURCES Configuration.cpp
        utils/XMLUtils.cpp
    HEADERS Configuration.hpp
        utils/XMLUtils.hpp
    DEPS_PKGCONFIG
        libxml-2.0
        base-logging
)
target_link_libraries(qxcfg ${Boost_LIBRARIES})
