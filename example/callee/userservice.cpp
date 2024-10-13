#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
using namespace std;
/*
USerServiceԭ����һ�����ط����ṩ�����������ڵı��ط�����Login��GetFriendLists
*/
class UserService : public fixbug::UserServiceRpc  // ʹ����rpc����ķ����ˣ�rpc�����ṩ�ߣ�
{
   public:
    // ��дUserServiceRpc����麯��
    bool Login(string name, string pwd) 
    {
        cout << "doing local service: Login" << endl;
        cout << "name" << name << "pwd" << pwd << endl;
        return true;
    }
    /*
    ������Щ���������ɿ��ֱ�ӵ��õ�
    caller ==> Login(LoginRequest) ==> muduo ==> callee ==> Login(LoginRequest) ==>
    ����������д��Login����
    */
    void Login(::google::protobuf::RpcController* controller, const ::fixbug::LoginRequest* request,
               ::fixbug::LoginResponse* response, ::google::protobuf::Closure* done) {
        // ��ܸ�ҵ���ϱ����������LoginRequest, Ӧ�û�ȡ��Ӧ������������ҵ��
        string name = request->name();
        string pwd = request->pwd();

        // ������ҵ��
        bool login_result = Login(name, pwd);

        // ����Ӧд��, ���������롢������Ϣ������ֵ
        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        // ִ�лص�����, ִ����Ӧ�������ݵ����л������緢�ͣ������ɿ����ɵģ�
        done->Run();
    }
};

int main(int argv, char** argc)
{
    // ���ÿ�ܵĳ�ʼ������
    MprpcApplication::Init(argv, argc);

    // provider��һ��rpc����������, ��UserService���󷢲���rpc�ڵ���
    RpcProvider provider;
    provider.NotifyService(new UserService()); 

    // ����һ��rpc���񷢲��ڵ�, Run�Ժ���̽�������״̬���ȴ�Զ�̵�rpc��������
    provider.Run();


    return 0;
}
