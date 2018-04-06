#include <boost/test/unit_test.hpp>
#include <qxcfg/Dummy.hpp>

using namespace qxcfg;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    qxcfg::DummyClass dummy;
    dummy.welcome();
}
