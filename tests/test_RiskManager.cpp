#include <QtTest/QtTest>
#include "../include/RiskManager.h"
#include "../include/ConfigManager.h"

class TestRiskManager : public QObject {
    Q_OBJECT
private slots:
    void testConfigLoading();
    void testRiskChecks();
    void testLotSizeCalculation();
};

void TestRiskManager::testConfigLoading() {
    ConfigManager configMgr;
    configMgr.loadConfig("../config.json");
    RiskManager rm;
    // Simulate loading config
    rm.setMaxRiskPerTrade(configMgr.getConfig()["risk"].toObject()["maxRiskPerTrade"].toDouble());
    rm.setMaxDailyRisk(configMgr.getConfig()["risk"].toObject()["maxDailyRisk"].toDouble());
    QCOMPARE(rm.getMaxRiskPerTrade(), 2.0);
    QCOMPARE(rm.getMaxDailyRisk(), 10.0);
}

void TestRiskManager::testRiskChecks() {
    RiskManager rm;
    rm.setEquity(10000.0);
    rm.setMaxRiskPerTrade(2.0);
    rm.setMaxDailyRisk(10.0);
    QVERIFY(rm.isRiskAcceptable(1.0, 100.0)); // 100 risk, under 2% of 10k
    QVERIFY(!rm.isRiskAcceptable(10.0, 1000.0)); // 10k risk, over 2% of 10k
}

void TestRiskManager::testLotSizeCalculation() {
    RiskManager rm;
    rm.setEquity(10000.0);
    double lot = rm.calculateLotSize("BTCUSD", 100.0, 2.0);
    QVERIFY(lot > 0.0);
}

QTEST_MAIN(TestRiskManager)
#include "test_RiskManager.moc" 