#include <iostream>
#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "user.pb.h"
int main(int argc, char** argv) {
    // ��������������, ��ʹ��mprpc���������rpc�ķ������, һ��Ҫ����ÿ�ܵĳ�ʼ������
    MprpcApplication::Init(argc, argv);
    // �ͻ���ʹ�õ���UserServiceRpc_Stub
    // ��ʾ����Զ�̷�����rpc����
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());  // RpcChannel�������Ϊ�н�
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    // rpc��������Ӧ
    fixbug::LoginResponse response;
    // �ͻ��˵���Login, ʵ���϶��ǵ��øú���RpcChannel->RpcChannel::CallMethod
    stub.Login(nullptr, &request, &response, nullptr);

    // һ��rpc�������, �����ý��
    if (response.result().errcode() == 0) {
        // ������ȷ
        std::cout << "rpc login response:" << response.success() << std::endl;
    } else {
        // ����ʧ��
        std::cout << "rpc login response error:" << response.result().errmsg() << std::endl;
    }

    fixbug::RegisterRequest req;
    req.set_id(2000);
    req.set_name("mrpc");
    req.set_pwd("66666");
    fixbug::RegisterResponse rsp;
    stub.Register(nullptr, &req, &rsp, nullptr);

    if (rsp.result().errcode() == 0) {
        // ������ȷ
        std::cout << "rpc registerRegister response:" << rsp.success() << std::endl;
    } else {
        // ����ʧ��
        std::cout << "rpc register response error:" << rsp.result().errmsg() << std::endl;
    }
    
    return 0;
}