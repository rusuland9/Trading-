#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>

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
    void writeLog(const QString &level, const QString &message);
    
    QFile m_logFile;
    QTextStream m_stream;
    QMutex m_mutex;
    bool m_initialized;
};

#endif // LOGGER_H 