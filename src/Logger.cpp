#include "Logger.h"
#include <QThread>
#include <QQueue>
#include <QTimer>
#include <QFileInfo>
#include <QFile>

Logger::Logger(QObject *parent)
    : QObject(parent)
    , m_initialized(false)
    , m_workerThread(nullptr)
{
    m_workerThread = QThread::create([this]() { this->processQueue(); });
    m_workerThread->start();
}

Logger::~Logger()
{
    if (m_workerThread) {
        m_workerThread->requestInterruption();
        m_workerThread->quit();
        m_workerThread->wait();
        delete m_workerThread;
    }
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

void Logger::info(const QString &message)    { enqueueLog("INFO", message); }
void Logger::warning(const QString &message) { enqueueLog("WARNING", message); }
void Logger::error(const QString &message)   { enqueueLog("ERROR", message); }
void Logger::debug(const QString &message)   { enqueueLog("DEBUG", message); }

void Logger::exportLogs(const QString &filePath)
{
    QMutexLocker locker(&m_mutex);
    if (m_logFile.isOpen()) m_stream.flush();
    QFile::copy(m_logFile.fileName(), filePath);
}

void Logger::enqueueLog(const QString &level, const QString &message)
{
    QMutexLocker locker(&m_queueMutex);
    QString logEntry = QString("[%1] %2: %3\n")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
        .arg(level)
        .arg(message);
    m_logQueue.enqueue(logEntry);
}

void Logger::processQueue()
{
    while (!QThread::currentThread()->isInterruptionRequested()) {
        QString entry;
        {
            QMutexLocker locker(&m_queueMutex);
            if (!m_logQueue.isEmpty()) {
                entry = m_logQueue.dequeue();
            }
        }
        if (!entry.isEmpty()) {
            QMutexLocker locker(&m_mutex);
            if (m_initialized) {
                m_stream << entry;
                m_stream.flush();
            }
        } else {
            QThread::msleep(20); // Sleep briefly if no log
        }
    }
} 