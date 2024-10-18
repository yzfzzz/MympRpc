#include "mprpcchannel.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "mprpcapplication.h"
#include "rpcheader.pb.h"
/*
���ݸ�ʽ: header_size + service_name method_name args_size + args
*/
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                              google::protobuf::RpcController* controller,
                              const google::protobuf::Message* request,
                              google::protobuf::Message* response,
                              google::protobuf::Closure* done) {
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    // ��ȡ���������л��ַ������� args_size
    std::string args_str;
    int args_size = 0;
    // ���л�
    if (request->SerializeToString(&args_str)) {
        args_size = args_str.size();
    } else {
        std::cout << "serialize request error!" << std::endl;
        return;
    }

    // ����rpc������header
    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if (rpcHeader.SerializeToString(&rpc_header_str)) {
        header_size = rpc_header_str.size();
    } else {
        std::cout << "serialize rpc header error!" << std::endl;
        return;
    }
    // ��֯�����͵�rpc�����ַ���
    // ???header_size�����ƴ洢
    std::string send_rpc_str;
    // ��ʾ RPC ͷ����С�� uint32_t ���ͱ��� header_size �Զ�������ʽ�洢��һ���ַ�����
    send_rpc_str.insert(0, std::string((char*)&header_size, 4));
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;

    // ��ӡ������Ϣ
    std::cout << "=========================" << std::endl;
    std::cout << "header_size: " << header_size << std::endl;
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_str: " << args_str << std::endl;

    // ʹ��tcp���
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1) {
        std::cout << "error:" << errno << std::endl;
        close(clientfd);
        exit(EXIT_FAILURE);
    }

    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverip").c_str());

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // ����rpc����˿�
    if (connect(clientfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cout << "connect error! error:" << errno << std::endl;
        close(clientfd);
        exit(EXIT_FAILURE);
    }

    // ����rpc����
    if (send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0) == -1) {
        std::cout << "connect error! errno:" << errno << std::endl;
        close(clientfd);
        return;
    }

    // ?����Ҫ�����ȴ���
    // ����rpc�������Ӧֵ
    char recv_buf[1024] = {};
    int recv_size = 0;
    if ((recv_size = recv(clientfd, recv_buf, 1024, 0)) == -1) {
        std::cout << "recv error! errno:" << errno << std::endl;
        close(clientfd);
        return;
    }

    // �����л�rpc���õ���Ӧ����
    if (!response->ParseFromArray(recv_buf, recv_size)) {
        std::cout << "parse error! response str: " << recv_buf << std::endl;
        close(clientfd);
        return;
    }
    close(clientfd);
}