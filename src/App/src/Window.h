#pragma once
#include <string>
#include <tuple>
#include <memory>

class Window final
{
public:
    Window();
    ~Window();

    int init(unsigned width, unsigned height, const std::string &name);
    [[nodiscard]] bool shouldClose() const;

    void swapBuffers();
    void pollEvents();

    [[nodiscard]] std::tuple<std::uint16_t, std::uint16_t> size();

private:
    class WindowImpl;
    std::unique_ptr<WindowImpl> _p_impl;
};
