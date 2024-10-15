#pragma once
#include <google/protobuf/descriptor.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/TcpServer.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "google/protobuf/service.h"

// ����ṩ��ר�ŷ��񷢲�rpc��������������
class RpcProvider {
   public:
    // �����ǿ�ܸ��ⲿʹ�õ�, ���Է���rpc�����ĺ����ӿ�
    // google::protobuf::Service ==> UserServiceRpc ==> UserService
    void NotifyService(google::protobuf::Service* service);

    // ����rpc����ڵ㣬��ʼԶ�̵���
    void Run();

   private:
    // ���EventLoop
    muduo::net::EventLoop m_eventLoop;

    // service����������Ϣ
    struct ServiceInfo {
        google::protobuf::Service* m_service;  // ����������
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*>
            m_methodMap;  // ������񷽷�
    };
    // �洢ע��ɹ��ķ�����������񷽷���������Ϣ
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;

    // �µ�socket���ӻص�
    void OnConnection(const muduo::net::TcpConnectionPtr& conn);

    // ���ֻ������ɶ�д, ��ִ�иûص�����
    void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);
};