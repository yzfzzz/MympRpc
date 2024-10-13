#pragma once

// mprpcøÚº‹≥ı ºªØ¿‡
class MprpcApplication
{
    public:
        static void Init(int argc, char** argvc);
        static MprpcApplication& GetInstance()
        {
            static MprpcApplication app;
            return app;
        }
    private:
        MprpcApplication();
        MprpcApplication(const MprpcApplication&) = delete;
        MprpcApplication(MprpcApplication&&) = delete;
};