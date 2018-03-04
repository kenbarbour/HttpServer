#include "catch.hpp"
#include "WebKernel.h"

TEST_CASE("Test getServer","[WebKernel]")
{
    WebKernel kernel(80);
    kernel.getServer();
}
