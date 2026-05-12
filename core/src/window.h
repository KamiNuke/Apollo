#pragma once
#include <string>

namespace Apollo
{
    class Window
    {
    public:
        struct Properties
        {
            std::string title;
            int width;
            int height;

            Properties(const std::string& _title = "Apollo",
                        const int _width = 1280,
                        const int _height = 720)
                : title(_title), width(_width), height(_height)
            {
            }
        };

        virtual ~Window() {}

        virtual void update() {};

        [[nodiscard]] virtual int getWidth() const = 0;
        [[nodiscard]] virtual int getHeight() const = 0;

        static Window* create(const Properties& props = Properties());
    private:

    };
}