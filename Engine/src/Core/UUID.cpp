#include "UUID.h"
#include <random>

namespace Apollo
{
    static std::random_device s_rd{};
    static std::mt19937 s_engine(s_rd());
    static std::uniform_int_distribution<uint64_t> s_uniformDistribution;

    UUID::UUID()
        : m_UUID(s_uniformDistribution(s_engine))
    {
    }

    UUID::UUID(uint64_t uuid)
        : m_UUID(uuid)
    {
    }
}
