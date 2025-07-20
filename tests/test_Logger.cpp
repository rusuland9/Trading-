#include <QtTest/QtTest>
#include "../include/Logger.h"

class TestLogger : public QObject {
    Q_OBJECT
private slots:
    void testAsyncLogging();
};

void TestLogger::testAsyncLogging() {
    Logger logger;
    logger.initialize("test_log.log");
    logger.info("Test info message");
    logger.error("Test error message");
    QTest::qWait(100); // Wait for async flush
    QFile file("test_log.log");
    QVERIFY(file.open(QIODevice::ReadOnly));
    QByteArray content = file.readAll();
    QVERIFY(content.contains("Test info message"));
    QVERIFY(content.contains("Test error message"));
    file.close();
    QFile::remove("test_log.log");
}

QTEST_MAIN(TestLogger)
#include "test_Logger.moc" 