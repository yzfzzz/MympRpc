#include "rpcprovider.h"
#include "mprpcapplication.h"
#include "rpcheader.pb.h"
/*
service_name => service描述
                        ==> service* 记录服务对象
                        method_name => method方法对象
*/
void RpcProvider::NotifyService(google::protobuf::Service* service) {
    ServiceInfo service_info;
    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor* pserviceDesc = service->GetDescriptor();
    // 获取服务的名字
    std::string service_name = pserviceDesc->name();
    // 获取服务对象service的方法数量
    int methodCnt = pserviceDesc->method_count();

    std::cout << "service_name: " << service_name << std::endl;

    for (int i = 0; i < methodCnt; i++) {
        // 获取了服务对象指定下标的服务方法的描述(抽象描述)
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name, pmethodDesc});
        std::cout << "method_name: " << method_name << std::endl;
    }

    service_info.m_service = service;
    m_serviceMap.insert({service_name, service_info});
}

void RpcProvider::Run() {
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverip").c_str());
    muduo::net::InetAddress address(ip, port);

    // ------------------------muduo------------------------------------
    // 启动Tcp Server
    // 创建TcpServer对象
    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");
    // 绑定连接回调和消息回调方法, 分离了网络代码和业务代码
    // 将RpcProvider::OnConnection(conn)中的参数设置为this
    server.setConnectionCallback(
        std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));

    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1,
                                        std::placeholders::_2, std::placeholders::_3));
    // 设置muduo库的线程数量
    server.setThreadNum(4);

    // 启动网络服务
    server.start();
    m_eventLoop.loop();
}

/*
在框架内部, RpcProvider和RpcConsumer协商好之间通信用的protobuf数据类型
service_name method_name args       定义proto的message类型，进行数据的序列化和反序列化
数据格式: header_size + header_str + args_str
16UserServiceLoginzhang san123456

*/

// 新的socket连接回调
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn) {
    if(!conn->connected())
    {
        // 和rpc client的连接断开了
        conn->shutdown();
    }
}

// 已建立连接用户的读写事件回调, 如果远程有一个rpc服务的调用请求, 那么OnMessage方法会响应 ==> 黄色部分
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer,
                            muduo::Timestamp) 
{
    // 网络上接收的远程rpc调用请求的字符流
    std::string recv_buf = buffer->retrieveAllAsString();

    // 从字符流中读取前4个字节的内容, 将数据拷贝到header_size的地址处
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0);

    // 根据header_size读取数据头的原始字符流
    std::string rpc_header_str = recv_buf.substr(4, header_size);

    // 反序列化数据, 得到rpc请求的详细信息
    mprpc::RpcHeader rpcHeader;

    std::string service_name;
    std::string method_name;
    uint32_t args_size;

    if(rpcHeader.ParseFromString(rpc_header_str))
    {
        // 数据头反序列化成功
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    }
    else
    {
        // 数据头反序列化失败
        std::cout << "rpc_head_str: "<< rpc_header_str << "parse error!" << std::endl;
    }
    // 获取rpc方法参数的字节流数据
    std::string args_str = recv_buf.substr(4+header_size, args_size);

    // 打印调试信息
    std::cout << "=========================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl;
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_str: " << args_str << std::endl;
}