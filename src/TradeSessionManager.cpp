#include "TradeSessionManager.h"

TradeSessionManager::TradeSessionManager(QObject *parent)
    : QObject(parent)
    , m_sessionStart(QTime(9, 0))
    , m_sessionEnd(QTime(17, 0))
    , m_intradayOnly(true)
    , m_sessionTimer(nullptr)
{
    m_sessionTimer = new QTimer(this);
    m_sessionTimer->setInterval(60000); // Check every minute
    connect(m_sessionTimer, &QTimer::timeout, this, [this]() {
        if (isInTradingSession()) {
            emit sessionStarted();
        } else {
            emit sessionEnded();
        }
    });
    m_sessionTimer->start();
}

void TradeSessionManager::setTradingSession(const QTime &start, const QTime &end)
{
    m_sessionStart = start;
    m_sessionEnd = end;
}

bool TradeSessionManager::isInTradingSession() const
{
    QTime currentTime = QTime::currentTime();
    return currentTime >= m_sessionStart && currentTime <= m_sessionEnd;
}

void TradeSessionManager::setIntradayOnly(bool enabled)
{
    m_intradayOnly = enabled;
} 