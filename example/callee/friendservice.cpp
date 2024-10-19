#include <iostream>
#include <string>
#include <vector>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
class FriendService : public fixbug::FriendServiceRpc {
   public:
    std::vector<std::string> GetFriendsList(uint32_t userid) {
        std::cout << "do GetFriendsList service! User id:"<< userid << std::endl;
        std::vector<std::string> vec;
        vec.push_back("zhang san");
        vec.push_back("li si");
        vec.push_back("wang wu");
        return vec;
    }

    // ��д���෽��
    void GetFriendsList(::google::protobuf::RpcController* controller,
                        const ::fixbug::GetFriendListsRequest* request,
                        ::fixbug::GetFriendListsResponse* response,
                        ::google::protobuf::Closure* done) {
        uint32_t userid = request->userid();
        std::vector<std::string> friendsList = GetFriendsList(userid);
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        for (std::string& name : friendsList) {
            std::string* p = response->add_friends();
            *p = name;
        }
        done->Run();
    }
};

int main(int argv, char** argc) {
    // ���ÿ�ܵĳ�ʼ������
    MprpcApplication::Init(argv, argc);

    RpcProvider provider;
    provider.NotifyService(new FriendService());

    // ����һ��rpc���񷢲��ڵ�, Run�Ժ���̽�������״̬���ȴ�Զ�̵�rpc��������
    provider.Run();

    return 0;
}