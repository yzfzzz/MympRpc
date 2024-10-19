#include <iostream>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"
int main(int argc, char** argv) {
    // 整个程序启动后, 想使用mprpc框架来享受rpc的服务调用, 一定要想调用框架的初始化函数
    MprpcApplication::Init(argc, argv);
    // 客户端使用的是FriendServiceRpc_Stub
    // 演示调用远程发布的rpc方法
    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());  // RpcChannel可以理解为中介
    fixbug::GetFriendListsRequest request;
    request.set_userid(1000);
    // rpc方法的响应
    fixbug::GetFriendListsResponse response;
    // 客户端调用etFriendsList, 实际上都是调用该函数RpcChannel->RpcChannel::CallMethod
    stub.GetFriendsList(nullptr, &request, &response, nullptr);

    // 一次rpc调用完成, 读调用结果
    if (response.result().errcode() == 0) {
        // 调用正确
        std::cout << "rpc GetFriendsList response success!" << std::endl;
        int size = response.friends_size();
        for (int i = 0; i < size; i++) {
            std::cout << "index:" << (i + 1) << "name:" << response.friends(i) << std::endl;
        }
    } else {
        // 调用失败
        std::cout << "rpc GetFriendsList response error:" << response.result().errmsg()
                  << std::endl;
    }

    return 0;
}