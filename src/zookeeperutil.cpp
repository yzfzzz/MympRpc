#include "zookeeperutil.h"
#include <semaphore.h>
#include <iostream>
#include "mprpcapplication.h"

ZkClient::ZkClient() : m_zhandle(nullptr) {}

ZkClient::~ZkClient() {
    if (m_zhandle != nullptr) {
        // �رվ��, �ͷ���Դ
        zookeeper_close(m_zhandle);
    }
}

// ȫ�ֵ�watcher�۲���  zkserver��zkclient��֪ͨ
void global_watcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx) 
{
    // �ص�����Ϣ�����ǺͻỰ��ص���Ϣ����
    if(type == ZOO_SRSSION_EVENT)
    {
        // zkclient��zkserver���ӳɹ�
        if(state == ZOO_CONNECTED_STATE)
        {
            sem_t* sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

void ZkClient::Start() {
    std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;
    /*
    zookeeper_mt: ���̰߳汾
    zookeeper��api�ͻ����ṩ�������߳�
    api�����߳�
    ����I/O�߳� pthread_create poll
    watcher�ص��߳�
    */
    m_zhandle = zookeeper_init(connstr.c_str(), , 30000, nullptr, nullptr, 0);
    if (m_zhandle == nullptr) {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    // ��ʱ��������ɹ���

    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);
    std::cout << "zookeeper_init success!" << std::endl;
}

void ZkClient::Create(const char* path, const char* data, int datalen, int state)
{
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;
    // ���ж�path��ʾ��znode�ڵ��Ƿ����, �������, �Ͳ��ظ�������
    flag = zoo_exists(m_zhandle, path, 0, nullptr);
    // �ڵ㲻����
    if(ZNONODE == flag) 
    {
        // ����ָ����path��znode�ڵ�
        flag = zoo_create(m_zhandle, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
        if(flag == ZOK)
        {
            std::cout << "znode create success... path:" << path << std::endl;
        } 
        else
        {
            std::cout << "flag:" << flag << std::endl;
            std::cout << "znode create error... path:" << path << std::endl; 
            exit(EXIT_FAILURE);
        }
    }
}

std::string ZkClient::GetData(const char* path)
{
    char buffer[64];
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(m_zhandle, path, 0,buffer, &bufferlen, nullptr);
    if(flag != ZOK)
    {
        std::cout << "get znode error... path" << path << std::endl;
        return "";
    }
    else
    {
        return buffer;
    }
}