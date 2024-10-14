#pragma once
#include <unordered_map>

// ��ܶ�ȡ�����ļ���
class MprpcConfig
{
public:
    //  ���ӽ������������ļ�    
    void LoadConfigFile(const char* config_file);
    // ��ѯ��������Ϣ
    std::string Load(const std::string& key);

private:
    std::unordered_map<std::string, std::string> m_configMap;
};