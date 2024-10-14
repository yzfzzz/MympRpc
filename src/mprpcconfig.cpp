#include "mprpcconfig.h"
#include <iostream>
//  ���ӽ������������ļ�    
void MprpcConfig::LoadConfigFile(const char* config_file)
{
    FILE *pf = fopen(config_file, "r");
    if(pf == nullptr)
    {
        std::cout << config_file << "is not exist" << std::endl;
        exit(EXIT_FAILURE);
    }
    // 1.ע�� 2.��ȷ�������� 3.ȥ������Ŀո�
    while (!feof(pf))
    {
        char buf[512] = {0};
        fgets(buf, 512, pf);

        // ȥ���ַ���ǰ�����Ŀո�
        std::string src_buf(buf);
        // �ҵ���һ����Ϊ�ո������
        int idx = src_buf.find_first_not_of(' ');
        if(idx != -1)
        {
            // ˵���ַ���ǰ���пո�
            src_buf = src_buf.substr(idx, src_buf.size()-idx);
        }

        // �ҵ����һ����Ϊ�ո������
        idx = src_buf.find_last_not_of(' ');
        if(idx != -1)
        {
            // ˵���ַ��������пո�
            src_buf = src_buf.substr(0, idx+1);
        }

        // �ж�#��ע�ͻ��߿���
        if(src_buf[0] == '#' || src_buf.empty())
        {
            continue;
        }

        // ����������
        idx = src_buf.find('=');
        if(idx == -1)
        {
           // ������Ϸ�
           continue;
        }

        std::string value;
        std::string key;

        key = src_buf.substr(0, idx);
        value = src_buf.substr(idx+1, src_buf.size()-idx);
        m_configMap.insert(std::pair<std::string,std::string>(key,value));

    }
    
}
// ��ѯ��������Ϣ
std::string MprpcConfig::Load(const std::string& key)
{
    auto it = m_configMap.find(key);
    if(it == m_configMap.end())
    {
        return "";
    }
    return it->second;
}
