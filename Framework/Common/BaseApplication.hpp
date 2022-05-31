#pragma once

#include "IApplication.hpp"

namespace My {
    class BaseApplication : implements IApplication {
    public:
        // Override
        virtual int Initialize();
        virtual void Finalize();
        virtual void Tick();
        virtual bool IsQuit();
    protected:
        // Flag indicates whether the main loop needs to quit
        bool m_bQuit;
    };
}