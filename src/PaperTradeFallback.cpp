#include "PaperTradeFallback.h"

PaperTradeFallback::PaperTradeFallback(QObject *parent)
    : QObject(parent)
    , m_paperTradingActive(false)
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