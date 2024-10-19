#include <iostream>
#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "user.pb.h"
int main(int argc, char** argv) {
    // 整个程序启动后, 想使用mprpc框架来享受rpc的服务调用, 一定要想调用框架的初始化函数
    MprpcApplication::Init(argc, argv);
    // 客户端使用的是UserServiceRpc_Stub
    // 演示调用远程发布的rpc方法
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());  // RpcChannel可以理解为中介
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    // rpc方法的响应
    fixbug::LoginResponse response;
    // 客户端调用Login, 实际上都是调用该函数RpcChannel->RpcChannel::CallMethod
    stub.Login(nullptr, &request, &response, nullptr);

    // 一次rpc调用完成, 读调用结果
    if (response.result().errcode() == 0) {
        // 调用正确
        std::cout << "rpc login response:" << response.success() << std::endl;
    } else {
        // 调用失败
        std::cout << "rpc login response error:" << response.result().errmsg() << std::endl;
    }

    fixbug::RegisterRequest req;
    req.set_id(2000);
    req.set_name("mrpc");
    req.set_pwd("66666");
    fixbug::RegisterResponse rsp;
    stub.Register(nullptr, &req, &rsp, nullptr);

    if (rsp.result().errcode() == 0) {
        // 调用正确
        std::cout << "rpc registerRegister response:" << rsp.success() << std::endl;
    } else {
        // 调用失败
        std::cout << "rpc register response error:" << rsp.result().errmsg() << std::endl;
    }
    
    return 0;
}