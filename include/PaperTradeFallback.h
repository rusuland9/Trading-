#ifndef PAPERTRADEFALLBACK_H
#define PAPERTRADEFALLBACK_H

#include <QObject>

class PaperTradeFallback : public QObject
{
    Q_OBJECT

public:
    explicit PaperTradeFallback(QObject *parent = nullptr);
    ~PaperTradeFallback() = default;
    
    void activatePaperTrading();
    void deactivatePaperTrading();
    bool isPaperTradingActive() const { return m_paperTradingActive; }
    void notifyTradeResult(bool wasLoss);
    void resetFallback();
    
private:
    bool m_paperTradingActive;
    int m_consecutiveLosses;
    int m_lossThreshold;
};

#endif // PAPERTRADEFALLBACK_H 