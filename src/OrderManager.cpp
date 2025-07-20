#include "OrderManager.h"
#include "ExchangeConnector.h"

OrderManager::OrderManager(QObject *parent)
    : QObject(parent)
    , m_exchangeConnector(nullptr)
    , m_tickBuffer(0)
    , m_pendingTicks(0)
{
}

void OrderManager::setExchangeConnector(ExchangeConnector *connector)
{
    QMutexLocker locker(&m_mutex);
    m_exchangeConnector = connector;
}

void OrderManager::setTickBuffer(int buffer)
{
    QMutexLocker locker(&m_mutex);
    m_tickBuffer = buffer;
}

void OrderManager::onTick()
{
    QMutexLocker locker(&m_mutex);
    if (m_tickBuffer > 0) {
        m_pendingTicks++;
        if (m_pendingTicks < m_tickBuffer) return;
        m_pendingTicks = 0;
    }
    // Place all buffered orders
    for (const auto &order : m_orderBuffer) {
        if (m_exchangeConnector) {
            QString orderId = m_exchangeConnector->placeOrder(order);
            emit orderPlaced(orderId);
        }
    }
    m_orderBuffer.clear();
}

void OrderManager::placeOrder(const QString &symbol, const QString &side, double quantity, double price)
{
    QMutexLocker locker(&m_mutex);
    if (!m_exchangeConnector) return;
    OrderRequest req;
    req.symbol = symbol;
    req.side = (side == "BUY") ? OrderSide::BUY : OrderSide::SELL;
    req.type = OrderType::MARKET;
    req.quantity = quantity;
    req.price = price;
    if (m_tickBuffer > 0) {
        m_orderBuffer.push_back(req);
        return;
    }
    QString orderId = m_exchangeConnector->placeOrder(req);
    emit orderPlaced(orderId);
}

void OrderManager::cancelOrder(const QString &orderId)
{
    QMutexLocker locker(&m_mutex);
    if (m_exchangeConnector) {
        m_exchangeConnector->cancelOrder(orderId);
        emit orderCancelled(orderId);
    }
}

void OrderManager::modifyOrder(const QString &orderId, double newPrice)
{
    QMutexLocker locker(&m_mutex);
    if (m_exchangeConnector) {
        m_exchangeConnector->modifyOrder(orderId, newPrice);
    }
} 