#include "PaperTradeFallback.h"
#include <QDebug>

PaperTradeFallback::PaperTradeFallback(QObject *parent)
    : QObject(parent)
    , m_paperTradingActive(false)
    , m_consecutiveLosses(0)
    , m_lossThreshold(3) // Example: switch after 3 consecutive losses
{
}

void PaperTradeFallback::activatePaperTrading()
{
    m_paperTradingActive = true;
}

void PaperTradeFallback::deactivatePaperTrading()
{
    m_paperTradingActive = false;
}

void PaperTradeFallback::notifyTradeResult(bool wasLoss)
{
    if (wasLoss) {
        m_consecutiveLosses++;
        if (m_consecutiveLosses >= m_lossThreshold) {
            activatePaperTrading();
            qDebug() << "Paper trading activated due to consecutive losses.";
        }
    } else {
        m_consecutiveLosses = 0;
    }
}

void PaperTradeFallback::resetFallback()
{
    m_consecutiveLosses = 0;
    deactivatePaperTrading();
} 