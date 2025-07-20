#include <QtTest/QtTest>
#include "../include/StrategyEngine.h"
#include "../include/ConfigManager.h"

class TestStrategyEngine : public QObject {
    Q_OBJECT
private slots:
    void testConfigLoading();
    void testRenkoBrickFormation();
    void testPatternDetection();
};

void TestStrategyEngine::testConfigLoading() {
    ConfigManager configMgr;
    configMgr.loadConfig("../config.json");
    StrategyEngine engine;
    engine.loadConfig(configMgr.getConfig());
    QCOMPARE(engine.getBrickSize(), 10.0);
    QCOMPARE(engine.isSetup1Enabled(), true);
    QCOMPARE(engine.isSetup2Enabled(), true);
}

void TestStrategyEngine::testRenkoBrickFormation() {
    StrategyEngine engine;
    engine.setBrickSize(10.0);
    QDateTime now = QDateTime::currentDateTime();
    engine.processPriceData(100.0, now);
    engine.processPriceData(110.0, now.addSecs(1));
    engine.processPriceData(120.0, now.addSecs(2));
    auto bricks = engine.getRenkoBricks();
    QVERIFY(bricks.size() >= 2);
    QCOMPARE(bricks.back().close, 120.0);
}

void TestStrategyEngine::testPatternDetection() {
    StrategyEngine engine;
    engine.setBrickSize(10.0);
    QDateTime now = QDateTime::currentDateTime();
    // Two red, one green
    engine.processPriceData(100.0, now);
    engine.processPriceData(90.0, now.addSecs(1));
    engine.processPriceData(80.0, now.addSecs(2));
    engine.processPriceData(90.0, now.addSecs(3));
    QVERIFY(engine.detectTwoRedOneGreen());
    // Three green
    engine.processPriceData(100.0, now.addSecs(4));
    engine.processPriceData(110.0, now.addSecs(5));
    QVERIFY(engine.detectThreeBrickPattern());
}

QTEST_MAIN(TestStrategyEngine)
#include "test_StrategyEngine.moc" 