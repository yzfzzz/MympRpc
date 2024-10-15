#include "rpcprovider.h"
#include "mprpcapplication.h"

/*
service_name => service����
                        ==> service* ��¼�������
                        method_name => method��������
*/
void RpcProvider::NotifyService(google::protobuf::Service* service) {
    ServiceInfo service_info;
    // ��ȡ��������������Ϣ
    const google::protobuf::ServiceDescriptor* pserviceDesc = service->GetDescriptor();
    // ��ȡ���������
    std::string service_name = pserviceDesc->name();
    // ��ȡ�������service�ķ�������
    int methodCnt = pserviceDesc->method_count();

    std::cout << "service_name: " << service_name << std::endl;

    for (int i = 0; i < methodCnt; i++) {
        // ��ȡ�˷������ָ���±�ķ��񷽷�������(��������)
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
    // ����Tcp Server
    // ����TcpServer����
    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");
    // �����ӻص�����Ϣ�ص�����, ��������������ҵ�����
    // ��RpcProvider::OnConnection(conn)�еĲ�������Ϊthis
    server.setConnectionCallback(
        std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));

    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, std::placeholders::_1,
                                        std::placeholders::_2, std::placeholders::_3));
    // ����muduo����߳�����
    server.setThreadNum(4);

    // �����������
    server.start();
    m_eventLoop.loop();
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn) {}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*,
                            muduo::Timestamp) {}