#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
using namespace std;
/*
USerService原来是一个本地服务，提供了两个进程内的本地方法，Login和GetFriendLists
*/
class UserService : public fixbug::UserServiceRpc  // 使用在rpc服务的发布端（rpc服务提供者）
{
   public:
    // 重写UserServiceRpc类的虚函数
    bool Login(string name, string pwd) 
    {
        cout << "doing local service: Login" << endl;
        cout << "name" << name << "pwd" << pwd << endl;
        return true;
    }
    /*
    下面这些函数都是由框架直接调用的
    caller ==> Login(LoginRequest) ==> muduo ==> callee ==> Login(LoginRequest) ==>
    交到下面重写的Login方法
    */
    void Login(::google::protobuf::RpcController* controller, const ::fixbug::LoginRequest* request,
               ::fixbug::LoginResponse* response, ::google::protobuf::Closure* done) {
        // 框架给业务上报了请求参数LoginRequest, 应用获取响应的数据做本地业务
        string name = request->name();
        string pwd = request->pwd();

        // 做本地业务
        bool login_result = Login(name, pwd);

        // 把响应写入, 包括错误码、错误消息、返回值
        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        // 执行回调操作, 执行响应对象数据的序列化和网络发送（都是由框架完成的）
        done->Run();
    }
};

int main(int argv, char** argc)
{
    // 调用框架的初始化操作
    MprpcApplication::Init(argv, argc);

    // provider是一个rpc网络服务对象, 把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService()); 

    // 启动一个rpc服务发布节点, Run以后进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();


    return 0;
}
