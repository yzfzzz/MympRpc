#include "rpcprovider.h"
#include "mprpcapplication.h"
#include "rpcheader.pb.h"
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

/*
�ڿ���ڲ�, RpcProvider��RpcConsumerЭ�̺�֮��ͨ���õ�protobuf��������
service_name method_name args       ����proto��message���ͣ��������ݵ����л��ͷ����л�
���ݸ�ʽ: header_size + header_str + args_str
16UserServiceLoginzhang san123456

*/

// �µ�socket���ӻص�
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn) {
    if (!conn->connected()) {
        // ��rpc client�����ӶϿ���
        conn->shutdown();
    }
}

// �ѽ��������û��Ķ�д�¼��ص�, ���Զ����һ��rpc����ĵ�������, ��ôOnMessage��������Ӧ ==>
// ��ɫ����
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer,
                            muduo::Timestamp) {
    // �����Ͻ��յ�Զ��rpc����������ַ���
    std::string recv_buf = buffer->retrieveAllAsString();

    // ���ַ����ж�ȡǰ4���ֽڵ�����, �����ݿ�����header_size�ĵ�ַ��
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0);

    // ����header_size��ȡ����ͷ��ԭʼ�ַ���
    std::string rpc_header_str = recv_buf.substr(4, header_size);

    // �����л�����, �õ�rpc�������ϸ��Ϣ
    mprpc::RpcHeader rpcHeader;

    std::string service_name;
    std::string method_name;
    uint32_t args_size;

    if (rpcHeader.ParseFromString(rpc_header_str)) {
        // ����ͷ�����л��ɹ�
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    } else {
        // ����ͷ�����л�ʧ��
        std::cout << "rpc_head_str: " << rpc_header_str << "parse error!" << std::endl;
    }
    // ��ȡrpc�����������ֽ�������
    std::string args_str = recv_buf.substr(4 + header_size, args_size);

    // ��ӡ������Ϣ
    std::cout << "=========================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl;
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_str: " << args_str << std::endl;

    // ��ȡservice�����method����
    auto it = m_serviceMap.find(service_name);
    if (it == m_serviceMap.end()) {
        std::cout << service_name << "is not exist!" << std::endl;
        return;
    }

    auto mit = it->second.m_methodMap.find(method_name);
    if (mit == it->second.m_methodMap.end()) {
        std::cout << service_name << ": " << method_name << "is not exist!" << std::endl;
        return;
    }

    google::protobuf::Service* service = it->second.m_service;  // ��ȡservice���� new UserService
    const google::protobuf::MethodDescriptor* method = mit->second;  // ��ȡmethod���� Login

    // ����rpc�������õ�����request����Ӧresponse����
    google::protobuf::Message* request = service->GetRequestPrototype(method).New();
    // �����л�
    if (!request->ParseFromString(args_str)) {
        std::cout << "request  parse error! content: " << args_str << std::endl;
        return;
    }

    google::protobuf::Message* response = service->GetResponsePrototype(method).New();

    // �������method�����ĵ���, ��һ��Closure�Ļص�����
    google::protobuf::Closure* done =
        google::protobuf::NewCallback<RpcProvider, const muduo::net::TcpConnectionPtr&,
                                      google::protobuf::Message*>(
            this, &RpcProvider::SendRpcResponse, conn, response);

    // �ڿ���ϸ���Զ��rpc����, ���õ�ǰrpc�ڵ��Ϸ����ķ���
    // new UserService().Login(controller, request, response, done);
    service->CallMethod(method, nullptr, request, response, done);
}

void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn,
                                  google::protobuf::Message* response) {
    std::string response_str;
    if (response->SerializeToString(&response_str)) {
        // ���л��ɹ���, ͨ�������rpc����ִ�еĽ�����ͻ�rpc�ĵ��÷�
        conn->send(response_str);
    } else {
        std::cout << "serialize response_str error" << std::endl;
    }
    conn->shutdown();  // ģ��http�Ķ����ӷ���, ��rpcprovider�����Ͽ�����
}