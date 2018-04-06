// Do NOT add anything to this file
// This header from boost takes ages to compile, so we make sure it is compiled
// only once (here)
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "test_utils.hpp"

#define PROJECT_NAME std::string("qxcfg")

std::string getRootDir()
{
    char buffer[1024];
    BOOST_REQUIRE_MESSAGE( readlink("/proc/self/exe", buffer, 1024) != -1, "Retrieving current execution path");
    std::string str(buffer);
    std::string executionDir = str.substr(0, str.rfind(PROJECT_NAME + "/"));
    std::string configurationPath = executionDir + PROJECT_NAME + "/";
    return configurationPath;
}
