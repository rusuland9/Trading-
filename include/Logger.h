#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QThread>
#include <QQueue>

class Logger : public QObject
{
    Q_OBJECT

public:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();
    
    void initialize(const QString &logFilePath);
    void info(const QString &message);
    void warning(const QString &message);
    void error(const QString &message);
    void debug(const QString &message);
    void exportLogs(const QString &filePath);
    
private:
    void enqueueLog(const QString &level, const QString &message);
    void processQueue();
    
    QFile m_logFile;
    QTextStream m_stream;
    QMutex m_mutex;
    bool m_initialized;
    QThread *m_workerThread;
    QQueue<QString> m_logQueue;
    QMutex m_queueMutex;
};

#endif // LOGGER_H 