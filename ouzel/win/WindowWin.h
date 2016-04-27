// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <windows.h>
#include "Window.h"

namespace ouzel
{
    class WindowWin : public Window
    {
        friend Engine;
    public:
        virtual ~WindowWin();

        virtual void close() override;

        virtual void setSize(const Size2& newSize) override;
        virtual void setTitle(const std::string& newTitle) override;

        void handleResize(INT width, INT height);

        HWND getNativeWindow() const { return window; }
        HMONITOR getMonitor() const;

    protected:
        WindowWin(const Size2& size, bool resizable, bool fullscreen, const std::string& title, graphics::Renderer::Driver driver);
        virtual bool init() override;

        ATOM windowClass = 0;
        HWND window = 0;
        DWORD windowStyle;
    };
}
