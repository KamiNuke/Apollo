#include <logger/log.h>

int main(int argc, char* argv[])
{
    Apollo::Log::init();
    APOLLO_LOGGER_CRITICAL("FUCK");
    LOGGER_INFO("TEST");
    return 0;
}