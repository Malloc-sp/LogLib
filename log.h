#pragma once

#include <mutex>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>
#include <thread>
#include <chrono>
#include <iomanip>
#include <memory>

#define LOGGER_API __attribute__((visibility("default")))

using std::mutex;

enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class LOGGER_API Log
{
public:
    static Log& getInstance();
    void logBase(std::string str, LogLevel level);
    ~Log();
    Log(const Log&) = delete;                 // 禁止拷贝和赋值
    Log& operator=(const Log&) = delete;

    void setLogLevel(LogLevel level);
    bool setOutputFile(const std::string& filepath);// 设置输出文件（默认只输出到控制台）
    void setConsoleOutput(bool enable);// 启用/禁用控制台输出（默认启用）
    std::string currentTimeToString() const;    //输出当前时间
    std::string levelToString(LogLevel level) const;
    std::string execCmd(const std::string& cmd);    // 运行cmd命令

    void logDebug(std::string str);
    void logInfo(std::string str);
    void logWarning(std::string str);
    void logError(std::string str);

private:
    Log();

private:
    LogLevel m_curLevel = LogLevel::DEBUG;
    std::unique_ptr<std::ofstream> m_fileStream;        //日志文件流
    bool m_bConsoleOutput = true;
    mutex m_mutex;                     // 用于保护输出
    std::string logFilePath;                // 输出的文件路径
};

#define LOG_DEBUG(msg) Log::getInstance().logDebug(std::string("[")+__FILE__+":"+\
                std::to_string(__LINE__)+"]"+msg)
#define LOG_INFO(msg) Log::getInstance().logInfo(std::string("[")+__FILE__+":"+\
                std::to_string(__LINE__)+"]"+msg)
#define LOG_WARNING(msg) Log::getInstance().logWarning(std::string("[")+__FILE__+":"+\
                std::to_string(__LINE__)+"]"+msg)
#define LOG_ERROR(msg) Log::getInstance().logError(std::string("[")+__FILE__+":"+\
                std::to_string(__LINE__)+"]"+msg)
