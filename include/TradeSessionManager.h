#ifndef TRADESESSIONMANAGER_H
#define TRADESESSIONMANAGER_H

#include <QObject>
#include <QString>
#include <QTime>
#include <QTimer>

class TradeSessionManager : public QObject
{
    Q_OBJECT

public:
    explicit TradeSessionManager(QObject *parent = nullptr);
    ~TradeSessionManager() = default;
    
    void setTradingSession(const QTime &start, const QTime &end);
    bool isInTradingSession() const;
    void setIntradayOnly(bool enabled);
    
signals:
    void sessionStarted();
    void sessionEnded();
    
private:
    QTime m_sessionStart;
    QTime m_sessionEnd;
    bool m_intradayOnly;
    QTimer *m_sessionTimer;
};

#endif // TRADESESSIONMANAGER_H 