#ifndef RISKMANAGER_H
#define RISKMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QString>
#include <QTimer>
#include <QMutex>
#include <vector>
#include <map>

struct Position {
    QString symbol;
    QString side; // "BUY" or "SELL"
    double size;
    double entryPrice;
    double currentPrice;
    double stopLoss;
    double takeProfit;
    double unrealizedPnL;
    double realizedPnL;
    QDateTime openTime;
    QDateTime closeTime;
    bool isOpen;
    QString orderId;
};

struct RiskMetrics {
    double totalEquity;
    double availableMargin;
    double usedMargin;
    double dailyPnL;
    double weeklyPnL;
    double monthlyPnL;
    double maxDrawdown;
    double riskUsed;
    double riskRemaining;
    int openPositions;
    int dailyTrades;
    double winRate;
    double profitFactor;
    double sharpeRatio;
    QDateTime lastUpdate;
};

class RiskManager : public QObject
{
    Q_OBJECT

public:
    explicit RiskManager(QObject *parent = nullptr);
    ~RiskManager();
    
    // Risk parameters
    void setMaxRiskPerTrade(double percent);
    void setMaxDailyRisk(double percent);
    void setMaxOpenPositions(int count);
    void setMaxDrawdown(double percent);
    void setEquity(double equity);
    void setMaxTradesPerDay(int count);
    void setCounterTradingEnabled(bool enabled);
    void setTradesPerCounter(int count);
    
    // Position management
    bool canOpenPosition(const QString &symbol, double lotSize);
    double calculateLotSize(const QString &symbol, double stopLossPips, double riskPercent);
    double calculateMaxLotSize(const QString &symbol);
    bool shouldClosePosition(const QString &positionId);
    
    // Risk checks
    bool isRiskAcceptable(double lotSize, double stopLossPips);
    bool isDailyRiskExceeded();
    bool isDrawdownExceeded();
    bool isMaxTradesReached();
    
    // Position tracking
    void addPosition(const Position &position);
    void updatePosition(const QString &positionId, double currentPrice);
    void closePosition(const QString &positionId, double closePrice);
    void clearAllPositions();
    
    // Counter trading
    void startNewCounter();
    void endCurrentCounter();
    bool isCounterComplete();
    bool shouldContinueTrading();
    
    // Getters
    RiskMetrics getRiskMetrics() const;
    std::vector<Position> getOpenPositions() const;
    std::vector<Position> getClosedPositions() const;
    Position getPosition(const QString &positionId) const;
    
    double getEquity() const { return m_equity; }
    double getDailyPnL() const { return m_dailyPnL; }
    double getMaxDrawdown() const { return m_maxDrawdown; }
    double getRiskUsed() const { return m_riskUsed; }
    int getOpenPositionCount() const { return m_openPositions.size(); }
    int getDailyTradeCount() const { return m_dailyTradeCount; }
    
    // Risk limits
    double getMaxRiskPerTrade() const { return m_maxRiskPerTrade; }
    double getMaxDailyRisk() const { return m_maxDailyRisk; }
    int getMaxOpenPositions() const { return m_maxOpenPositions; }
    int getMaxTradesPerDay() const { return m_maxTradesPerDay; }

signals:
    void riskLimitReached(const QString &type);
    void positionOpened(const Position &position);
    void positionClosed(const Position &position);
    void positionUpdated(const Position &position);
    void riskMetricsUpdated(const RiskMetrics &metrics);
    void drawdownWarning(double currentDrawdown);
    void counterCompleted(int counterNumber, double pnl);
    void tradingHalted(const QString &reason);

private slots:
    void onUpdateRiskMetrics();
    void onCheckRiskLimits();
    void onUpdatePositions();

private:
    void calculateRiskMetrics();
    void updateDailyStatistics();
    void updateDrawdown();
    void checkRiskLimits();
    void resetDailyCounters();
    bool isNewTradingDay();
    
    // Helper methods
    double calculateUnrealizedPnL(const Position &position) const;
    double calculatePositionRisk(const Position &position) const;
    double getPipValue(const QString &symbol) const;
    double getMarginRequirement(const QString &symbol, double lotSize) const;
    
    // Member variables
    double m_equity;
    double m_initialEquity;
    double m_maxRiskPerTrade;
    double m_maxDailyRisk;
    int m_maxOpenPositions;
    double m_maxDrawdownPercent;
    int m_maxTradesPerDay;
    
    // Current state
    double m_dailyPnL;
    double m_weeklyPnL;
    double m_monthlyPnL;
    double m_maxDrawdown;
    double m_riskUsed;
    int m_dailyTradeCount;
    
    // Positions
    std::vector<Position> m_openPositions;
    std::vector<Position> m_closedPositions;
    std::map<QString, Position> m_positionMap;
    
    // Counter trading
    bool m_counterTradingEnabled;
    int m_tradesPerCounter;
    int m_currentCounterTrades;
    int m_counterNumber;
    double m_counterStartEquity;
    std::vector<double> m_counterResults;
    
    // Statistics
    int m_totalTrades;
    int m_winningTrades;
    double m_totalProfit;
    double m_totalLoss;
    double m_largestWin;
    double m_largestLoss;
    
    // Date tracking
    QDateTime m_lastTradingDay;
    QDateTime m_lastUpdate;
    
    // Timers
    QTimer *m_updateTimer;
    QTimer *m_riskCheckTimer;
    
    // Thread safety
    mutable QMutex m_mutex;
    
    // Constants
    static const double DEFAULT_MAX_RISK_PER_TRADE = 2.0; // 2%
    static const double DEFAULT_MAX_DAILY_RISK = 10.0; // 10%
    static const int DEFAULT_MAX_OPEN_POSITIONS = 5;
    static const double DEFAULT_MAX_DRAWDOWN = 20.0; // 20%
    static const int DEFAULT_MAX_TRADES_PER_DAY = 20;
    static const int DEFAULT_TRADES_PER_COUNTER = 10;
    
    // Update intervals
    static const int RISK_UPDATE_INTERVAL = 1000; // 1 second
    static const int POSITION_UPDATE_INTERVAL = 500; // 0.5 seconds
};

#endif // RISKMANAGER_H 