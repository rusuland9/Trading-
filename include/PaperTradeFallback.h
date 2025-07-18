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
    
private:
    bool m_paperTradingActive;
};

#endif // PAPERTRADEFALLBACK_H 