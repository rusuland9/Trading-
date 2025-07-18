#include "Logger.h"

Logger::Logger(QObject *parent)
    : QObject(parent)
    , m_initialized(false)
{
}

Logger::~Logger()
{
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

void Logger::initialize(const QString &logFilePath)
{
    QMutexLocker locker(&m_mutex);
    
    m_logFile.setFileName(logFilePath);
    if (m_logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        m_stream.setDevice(&m_logFile);
        m_initialized = true;
    }
}

void Logger::info(const QString &message)
{
    writeLog("INFO", message);
}

void Logger::warning(const QString &message)
{
    writeLog("WARNING", message);
}

void Logger::error(const QString &message)
{
    writeLog("ERROR", message);
}

void Logger::debug(const QString &message)
{
    writeLog("DEBUG", message);
}

void Logger::exportLogs(const QString &filePath)
{
    Q_UNUSED(filePath)
    // Stub implementation
}

void Logger::writeLog(const QString &level, const QString &message)
{
    if (!m_initialized) return;
    
    QMutexLocker locker(&m_mutex);
    
    QString logEntry = QString("[%1] %2: %3\n")
                      .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                      .arg(level)
                      .arg(message);
    
    m_stream << logEntry;
    m_stream.flush();
} 