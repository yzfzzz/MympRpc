#include "mprpclog.h"

// TODO: ����־ģ����뵽�����
void SetupLogging(const std::string logDir) {
    // ������־���Ŀ¼
    FLAGS_log_dir = logDir;

    // ������־����
    FLAGS_stderrthreshold = google::INFO;

    // ������־�ļ���СΪ10MB
    FLAGS_max_log_size = 10 * 1024;  // 10MB

    // ������־�ļ����еĻ�������׺
    FLAGS_alsologtostderr = true;
    FLAGS_logtostderr = false;

    FLAGS_colorlogtostderr = true;
    // ��־��໺���������0��ʾʵʱ���
    FLAGS_logbufsecs = 30;
}

void LogInit(char** argv, std::string logPath) {
    // ������־Ŀ¼
    std::string logDir = logPath;
    // ��ʼ����־
    SetupLogging(logDir);
    // ��ʼ�� Glog
    google::InitGoogleLogging(argv[0]);
}