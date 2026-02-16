#include "log.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

Log::Log()
{}

Log::~Log()
{
    if (m_fileStream && m_fileStream->is_open())
    {
        m_fileStream->close();
    }
}

Log& Log::getInstance()
{
    static Log instance;
    return instance;
}

void Log::logBase(std::string msg, LogLevel level)
{
    // 快速过滤，避免锁开销
    if (level < m_curLevel) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);   // 保证多线程输出不交错

    // 构建日志前缀: [时间] [线程ID] [级别] 文件名:行号
    std::ostringstream prefix;
    prefix << "[" << currentTimeToString() << "]"
           << "[" << levelToString(level) << "]";

    // 输出到控制台
    if (m_bConsoleOutput) {
        std::cout << prefix.str() << msg << std::endl;
    }

    // 输出到文件
    if (m_fileStream && m_fileStream->is_open()) {
        (*m_fileStream) << prefix.str() << msg << std::endl;
        m_fileStream->flush();   // 确保立即写入，避免程序崩溃丢失
    }
}

void Log::setLogLevel(LogLevel level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_curLevel = level;
}

bool Log::setOutputFile(const std::string &filepath)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto newStream = std::make_unique<std::ofstream>(filepath, std::ios::app);
    if (newStream->is_open())
    {
        m_fileStream = std::move(newStream);
        return true;
    }
    return false;
}

void Log::setConsoleOutput(bool enable)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_bConsoleOutput = enable;
}

std::string Log::currentTimeToString() const {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) % 1000;

    std::ostringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string Log::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

void Log::logDebug(std::string str)
{
    logBase(str, LogLevel::DEBUG);
}
void Log::logInfo(std::string str)
{
    logBase(str, LogLevel::INFO);
}
void Log::logWarning(std::string str)
{
    logBase(str, LogLevel::WARNING);
}
void Log::logError(std::string str)
{
    logBase(str, LogLevel::ERROR);
}
