#pragma once
#include <cstdint>

#include "Core/Base.h"

namespace Apollo
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual void Bind(uint32_t slot = 0) const = 0;

        [[nodiscard]] virtual uint32_t GetWidth() const = 0;
        [[nodiscard]] virtual uint32_t GetHeight() const = 0;
        [[nodiscard]] virtual const std::string& GetPath() const = 0;
        [[nodiscard]] virtual uint32_t GetID() const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(const std::string& path);
    };
} // Apollo
