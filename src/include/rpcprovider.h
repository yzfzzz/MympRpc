#pragma once
#include "google/protobuf/service.h"

// ����ṩ��ר�ŷ��񷢲�rpc��������������
class RpcProvider{
public:
    // �����ǿ�ܸ��ⲿʹ�õ�, ���Է���rpc�����ĺ����ӿ�
    // google::protobuf::Service ==> UserServiceRpc ==> UserService
    void NotifyService(google::protobuf::Service* service);

    // ����rpc����ڵ㣬��ʼԶ�̵���
    void Run(){}

};