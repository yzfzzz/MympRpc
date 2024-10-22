#include <glog/logging.h>
#include <iostream>
void SetupLogging(const std::string& logDir)
{
    // 设置日志输出目录
    FLAGS_log_dir = logDir;
    // google::SetLogDestination(google::INFO, "./logs/my_log_");

    // 设置日志级别
    FLAGS_stderrthreshold = google::INFO;

    // 设置日志文件大小为10MB
    FLAGS_max_log_size = 10 * 1024;  // 10MB

    // 禁用日志文件名中的机器名后缀
    // FLAGS_alsologtostderr = true;
    // FLAGS_logtostderr = false;

    FLAGS_colorlogtostderr = true;

    // 自动移除旧日志 day （apt旧版本没有）
    // google::EnableLogCleaner(3);
}

void logCondition()
{
    for (int i = 0; i < 10; ++i)
    {
        // 条件日志
        LOG_IF(INFO, i > 5) << "Got lots of cookies" << i;
        LOG_EVERY_N(INFO, 10) << "Got the " << google::COUNTER << "th cookie";
    }
}

int main(int argc, char *argv[])
{
    // 设置日志目录
    std::string logDir = "./logs";

    // 初始化日志
    SetupLogging(logDir);

    // 初始化 Glog
    std::cout << argv[0] << std::endl;
    google::InitGoogleLogging(argv[0]);

    // 示例输出不同级别的日志
    LOG(INFO) << "This is an informational message.";
    LOG(WARNING) << "This is a warning message.";
    LOG(ERROR) << "This is an error message.";
    // LOG(FATAL) << "This is a fatal error message.";

    logCondition();

    return 0;
}