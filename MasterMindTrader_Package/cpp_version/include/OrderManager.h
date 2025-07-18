#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QString>
#include <QDateTime>
#include <memory>
#include <vector>
#include <map>

class OrderManager : public QObject
{
    Q_OBJECT

public:
    explicit OrderManager(QObject *parent = nullptr);
    ~OrderManager() = default;
    
    void placeOrder(const QString &symbol, const QString &side, double quantity, double price);
    void cancelOrder(const QString &orderId);
    void modifyOrder(const QString &orderId, double newPrice);
    
signals:
    void orderPlaced(const QString &orderId);
    void orderFilled(const QString &orderId);
    void orderCancelled(const QString &orderId);
    
private:
    QMutex m_mutex;
};

#endif // ORDERMANAGER_H 