#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug;

int main()
{
    // LoginResponse rsp;
    // ResultCode* rc = rsp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("登录失败");

    GetFriendListsResponse rsp;
    ResultCode* rc = rsp.mutable_result();
    rc->set_errcode(0);

    User* user1 = rsp.add_friend_list();
    user1->set_age(20);
    user1->set_name("zhang san");
    user1->set_sex(User::MAN);

    User* user2 = rsp.add_friend_list();
    user2->set_age(50);
    user2->set_name("li si");
    user2->set_sex(User::WOMAN);

    std::cout << rsp.friend_list_size() << std::endl;

}


int test()
{
    // 初始化变量, 封装了login请求对象的数据
    LoginRequest req;
    req.set_name("zhang san");
    req.set_pwd("123456");

    // 对象数据序列化, => string
    std::string send_str;
    if(req.SerializeToString(&send_str))
    {
        std::cout << send_str << std::endl;
    }

    // 从send_str中反序列化一个login请求对象
    LoginRequest reqB;
    if(reqB.ParseFromString(send_str))
    {
        std::cout << reqB.name() << std::endl;
        std::cout << reqB.pwd() << std::endl;
    }


    return 0;
}