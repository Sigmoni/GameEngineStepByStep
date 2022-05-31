#pragma once

#include "IRuntimeModule.hpp"

namespace My {
    Interface IApplication : implements IRuntimeModule {
    public:
        // Override
        virtual int Initialize() = 0;
        virtual void Finalize() = 0;
        virtual void Tick() = 0;
        
        // Whether the app quits
        virtual bool IsQuit() = 0;
    };
}