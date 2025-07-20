#include <QtTest/QtTest>
#include "../include/PaperTradeFallback.h"

class TestPaperTradeFallback : public QObject {
    Q_OBJECT
private slots:
    void testFallbackActivation();
};

void TestPaperTradeFallback::testFallbackActivation() {
    PaperTradeFallback fallback;
    QVERIFY(!fallback.isPaperTradingActive());
    fallback.notifyTradeResult(true);
    fallback.notifyTradeResult(true);
    fallback.notifyTradeResult(true);
    QVERIFY(fallback.isPaperTradingActive());
    fallback.resetFallback();
    QVERIFY(!fallback.isPaperTradingActive());
}

QTEST_MAIN(TestPaperTradeFallback)
#include "test_PaperTradeFallback.moc" 