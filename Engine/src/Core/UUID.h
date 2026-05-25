#pragma once
#include <cstdint>

namespace Apollo
{
    class UUID
    {
    public:
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID&) = default;

        operator uint64_t() const { return m_UUID; }
    private:
        uint64_t m_UUID; // C++ standard does not require 128 bit unsigned integer
    };
}

namespace std
{
    template <typename T> struct hash;

    template<>
    struct hash<Apollo::UUID>
    {
        std::size_t operator()(const Apollo::UUID& uuid) const
        {
            return uuid;
        }
    };
}