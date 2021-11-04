#pragma once
#include <string>
#include <tuple>
#include <memory>

class Widget final
{
public:
    Widget();
    ~Widget();

    int init(unsigned width, unsigned height, const std::string &name);
    [[nodiscard]] bool shouldClose() const;

    void swapBuffers();
    void pollEvents();

    [[nodiscard]] std::tuple<std::uint16_t, std::uint16_t> size();

private:
    class WidgetImpl;
    std::unique_ptr<WidgetImpl> _p_impl;
};
