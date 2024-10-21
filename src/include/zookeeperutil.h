#pragma once
#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

// ��װ��zk�ͻ�����
class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    // ��������
    void Start();
    // ��zkserver�ϸ���path����znode�ڵ㣬state=0��ʾ�����Խڵ�
    void Create(const char* path, const char* data, int datalen, int state = 0);
    // ���ݲ���ָ����znode�ڵ�·������znode�ڵ��ֵ
    std::string GetData(const char* path);
private:
    // zk�ͻ��˵ľ��
    zhandle_t *m_zhandle;
}