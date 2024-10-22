#include <glog/logging.h>
#include <iostream>
void SetupLogging(const std::string& logDir)
{
    // ������־���Ŀ¼
    FLAGS_log_dir = logDir;
    // google::SetLogDestination(google::INFO, "./logs/my_log_");

    // ������־����
    FLAGS_stderrthreshold = google::INFO;

    // ������־�ļ���СΪ10MB
    FLAGS_max_log_size = 10 * 1024;  // 10MB

    // ������־�ļ����еĻ�������׺
    // FLAGS_alsologtostderr = true;
    // FLAGS_logtostderr = false;

    FLAGS_colorlogtostderr = true;

    // �Զ��Ƴ�����־ day ��apt�ɰ汾û�У�
    // google::EnableLogCleaner(3);
}

void logCondition()
{
    for (int i = 0; i < 10; ++i)
    {
        // ������־
        LOG_IF(INFO, i > 5) << "Got lots of cookies" << i;
        LOG_EVERY_N(INFO, 10) << "Got the " << google::COUNTER << "th cookie";
    }
}

int main(int argc, char *argv[])
{
    // ������־Ŀ¼
    std::string logDir = "./logs";

    // ��ʼ����־
    SetupLogging(logDir);

    // ��ʼ�� Glog
    std::cout << argv[0] << std::endl;
    google::InitGoogleLogging(argv[0]);

    // ʾ�������ͬ�������־
    LOG(INFO) << "This is an informational message.";
    LOG(WARNING) << "This is a warning message.";
    LOG(ERROR) << "This is an error message.";
    // LOG(FATAL) << "This is a fatal error message.";

    logCondition();

    return 0;
}