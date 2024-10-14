#pragma once
#include "mprpcconfig.h"
// mprpcøÚº‹≥ı ºªØ¿‡
class MprpcApplication
{
    public:
        static void Init(int argc, char** argv);
        static MprpcApplication& GetInstance()
        {
            static MprpcApplication app;
            return app;
        }
    private:
        static MprpcConfig m_config;
        MprpcApplication();
        MprpcApplication(const MprpcApplication&) = delete;
        MprpcApplication(MprpcApplication&&) = delete;
};