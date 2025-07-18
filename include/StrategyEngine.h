#ifndef STRATEGYENGINE_H
#define STRATEGYENGINE_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QMutex>
#include <vector>
#include <memory>

struct RenkoBrick {
    double open;
    double close;
    double high;
    double low;
    QDateTime timestamp;
    bool isGreen;
    bool isRed;
    double formationPercentage;
};

struct TradingSignal {
    enum Type { BUY, SELL, CLOSE };
    enum Setup { SETUP1, SETUP2 };
    
    Type type;
    Setup setup;
    QString symbol;
    double price;
    double stopLoss;
    double takeProfit;
    double lotSize;
    QDateTime timestamp;
    bool isValid;
    QString description;
};

class StrategyEngine : public QObject
{
    Q_OBJECT

public:
    explicit StrategyEngine(QObject *parent = nullptr);
    ~StrategyEngine();
    
    void start();
    void stop();
    void pause();
    void resume();
    
    bool isRunning() const { return m_running; }
    bool isPaused() const { return m_paused; }
    
    void setSymbol(const QString &symbol);
    void setBrickSize(double size);
    void setSetup1Enabled(bool enabled);
    void setSetup2Enabled(bool enabled);
    void setTickBuffer(int buffer);
    void setRiskPercent(double percent);
    
    QString getSymbol() const { return m_symbol; }
    double getBrickSize() const { return m_brickSize; }
    bool isSetup1Enabled() const { return m_setup1Enabled; }
    bool isSetup2Enabled() const { return m_setup2Enabled; }
    
    std::vector<RenkoBrick> getRenkoBricks() const;
    TradingSignal getLastSignal() const { return m_lastSignal; }
    
    // Statistics
    int getTotalSignals() const { return m_totalSignals; }
    int getSuccessfulSignals() const { return m_successfulSignals; }
    double getWinRate() const;

signals:
    void newSignal(const TradingSignal &signal);
    void brickFormed(const RenkoBrick &brick);
    void strategyStatusChanged(const QString &status);
    void errorOccurred(const QString &error);

private slots:
    void onProcessTick();
    void onAnalyzePattern();
    void onValidateSignal();

private:
    void initializeEngine();
    void processPriceData(double price, const QDateTime &timestamp);
    void formRenkoBrick(double price, const QDateTime &timestamp);
    void analyzeRenkoPattern();
    
    // Setup 1: Two red + green pattern
    TradingSignal analyzeSetup1();
    bool detectTwoRedOneGreen();
    
    // Setup 2: Three brick pattern
    TradingSignal analyzeSetup2();
    bool detectThreeBrickPattern();
    
    // Signal validation
    bool validateSignal(const TradingSignal &signal);
    double calculateLotSize(const TradingSignal &signal);
    double calculateStopLoss(const TradingSignal &signal);
    double calculateTakeProfit(const TradingSignal &signal);
    
    // Utility methods
    bool isGreenBrick(const RenkoBrick &brick) const;
    bool isRedBrick(const RenkoBrick &brick) const;
    double getBrickFormationPercentage(double currentPrice, double brickOpen) const;
    void updatePatternHistory();
    void logSignal(const TradingSignal &signal);
    
    // Member variables
    QString m_symbol;
    double m_brickSize;
    bool m_setup1Enabled;
    bool m_setup2Enabled;
    int m_tickBuffer;
    double m_riskPercent;
    
    bool m_running;
    bool m_paused;
    bool m_initialized;
    
    std::vector<RenkoBrick> m_renkoBricks;
    std::vector<TradingSignal> m_signalHistory;
    TradingSignal m_lastSignal;
    
    double m_currentPrice;
    QDateTime m_lastTickTime;
    
    // Pattern detection state
    bool m_inPattern;
    int m_patternBrickCount;
    double m_patternHighClose;
    double m_patternLowOpen;
    
    // Statistics
    int m_totalSignals;
    int m_successfulSignals;
    
    // Timers
    QTimer *m_tickTimer;
    QTimer *m_analysisTimer;
    QTimer *m_validationTimer;
    
    // Thread safety
    mutable QMutex m_mutex;
    
    // Configuration
    static const int MAX_BRICK_HISTORY = 100;
    static const int MAX_SIGNAL_HISTORY = 500;
    static const double MIN_BRICK_SIZE = 0.1;
    static const double MAX_BRICK_SIZE = 1000.0;
    static const double BRICK_FORMATION_THRESHOLD = 0.75; // 75%
};

#endif // STRATEGYENGINE_H 