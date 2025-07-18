#include "OrderManager.h"

OrderManager::OrderManager(QObject *parent)
    : QObject(parent)
{
}

void OrderManager::placeOrder(const QString &symbol, const QString &side, double quantity, double price)
{
    Q_UNUSED(symbol)
    Q_UNUSED(side)
    Q_UNUSED(quantity)
    Q_UNUSED(price)
    
    QString orderId = QString("ORDER_%1").arg(QDateTime::currentMSecsSinceEpoch());
    emit orderPlaced(orderId);
}

void OrderManager::cancelOrder(const QString &orderId)
{
    Q_UNUSED(orderId)
    emit orderCancelled(orderId);
}

void OrderManager::modifyOrder(const QString &orderId, double newPrice)
{
    Q_UNUSED(orderId)
    Q_UNUSED(newPrice)
} 