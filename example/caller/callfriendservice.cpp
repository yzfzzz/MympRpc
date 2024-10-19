#include <iostream>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"
int main(int argc, char** argv) {
    // ��������������, ��ʹ��mprpc���������rpc�ķ������, һ��Ҫ����ÿ�ܵĳ�ʼ������
    MprpcApplication::Init(argc, argv);
    // �ͻ���ʹ�õ���FriendServiceRpc_Stub
    // ��ʾ����Զ�̷�����rpc����
    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());  // RpcChannel�������Ϊ�н�
    fixbug::GetFriendListsRequest request;
    request.set_userid(1000);
    // rpc��������Ӧ
    fixbug::GetFriendListsResponse response;
    // �ͻ��˵���etFriendsList, ʵ���϶��ǵ��øú���RpcChannel->RpcChannel::CallMethod
    stub.GetFriendsList(nullptr, &request, &response, nullptr);

    // һ��rpc�������, �����ý��
    if (response.result().errcode() == 0) {
        // ������ȷ
        std::cout << "rpc GetFriendsList response success!" << std::endl;
        int size = response.friends_size();
        for (int i = 0; i < size; i++) {
            std::cout << "index:" << (i + 1) << "name:" << response.friends(i) << std::endl;
        }
    } else {
        // ����ʧ��
        std::cout << "rpc GetFriendsList response error:" << response.result().errmsg()
                  << std::endl;
    }

    return 0;
}