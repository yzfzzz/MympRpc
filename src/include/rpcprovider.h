#pragma once
#include "google/protobuf/service.h"

// 框架提供的专门服务发布rpc服务的网络对象类
class RpcProvider{
public:
    // 这里是框架给外部使用的, 可以发布rpc方法的函数接口
    // google::protobuf::Service ==> UserServiceRpc ==> UserService
    void NotifyService(google::protobuf::Service* service);

    // 启动rpc服务节点，开始远程调用
    void Run(){}

};