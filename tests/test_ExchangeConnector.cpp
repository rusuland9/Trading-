#include <QtTest/QtTest>
#include "../include/ExchangeConnector.h"

class TestExchangeConnector : public QObject {
    Q_OBJECT
private slots:
    void testTestModeOrderPlacement();
    void testAccountInfo();
};

void TestExchangeConnector::testTestModeOrderPlacement() {
    ExchangeConnector connector;
    connector.setTestMode(true);
    OrderRequest req;
    req.symbol = "BTCUSD";
    req.type = OrderType::MARKET;
    req.side = OrderSide::BUY;
    req.quantity = 1.0;
    req.price = 50000.0;
    QString orderId = connector.placeOrder(req);
    QVERIFY(!orderId.isEmpty());
}

void TestExchangeConnector::testAccountInfo() {
    ExchangeConnector connector;
    connector.setTestMode(true);
    AccountInfo info = connector.getAccountInfo();
    QCOMPARE(info.totalBalance, 10000.0);
    QCOMPARE(info.currency, QString("USD"));
}

QTEST_MAIN(TestExchangeConnector)
#include "test_ExchangeConnector.moc" 