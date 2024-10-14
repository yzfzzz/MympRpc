#pragma once
#include "mprpcconfig.h"
// mprpc��ܳ�ʼ����
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