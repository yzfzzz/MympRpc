#pragma once
#include <google/protobuf/service.h>
#include <string>

class MprpcController : public google::protobuf::RpcController {
   public:
    MprpcController();
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void SetFailed(const std::string& reason);

    // Ŀǰδʵ�־���Ĺ���
    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure* callback);

   private:
    bool m_failed;          // RPC����ִ�й����е�״̬
    std::string m_errText;  // RPC����ִ�еĴ�����Ϣ
};