#include "RiskManager.h"
#include <QJsonObject>

RiskManager::RiskManager(QObject *parent)
    : QObject(parent)
    , m_equity(10000.0)
    , m_initialEquity(10000.0)
    , m_maxRiskPerTrade(DEFAULT_MAX_RISK_PER_TRADE)
    , m_maxDailyRisk(DEFAULT_MAX_DAILY_RISK)
    , m_maxOpenPositions(DEFAULT_MAX_OPEN_POSITIONS)
    , m_maxDrawdownPercent(DEFAULT_MAX_DRAWDOWN)
    , m_maxTradesPerDay(DEFAULT_MAX_TRADES_PER_DAY)
    , m_dailyPnL(0.0)
    , m_weeklyPnL(0.0)
    , m_monthlyPnL(0.0)
    , m_maxDrawdown(0.0)
    , m_riskUsed(0.0)
    , m_dailyTradeCount(0)
    , m_counterTradingEnabled(false)
    , m_tradesPerCounter(DEFAULT_TRADES_PER_COUNTER)
    , m_currentCounterTrades(0)
    , m_counterNumber(0)
    , m_counterStartEquity(0.0)
    , m_totalTrades(0)
    , m_winningTrades(0)
    , m_totalProfit(0.0)
    , m_totalLoss(0.0)
    , m_largestWin(0.0)
    , m_largestLoss(0.0)
{
    // Initialize with default values
}

RiskManager::~RiskManager()
{
    // Clean up
}

void RiskManager::loadConfig(const QJsonObject &config)
{
    if (config.contains("risk")) {
        QJsonObject risk = config["risk"].toObject();
        if (risk.contains("maxRiskPerTrade")) setMaxRiskPerTrade(risk["maxRiskPerTrade"].toDouble());
        if (risk.contains("maxDailyRisk")) setMaxDailyRisk(risk["maxDailyRisk"].toDouble());
        if (risk.contains("maxOpenPositions")) setMaxOpenPositions(risk["maxOpenPositions"].toInt());
        if (risk.contains("maxTradesPerDay")) setMaxTradesPerDay(risk["maxTradesPerDay"].toInt());
        if (risk.contains("maxDrawdown")) setMaxDrawdown(risk["maxDrawdown"].toDouble());
    }
    if (config.contains("capital")) {
        QJsonObject capital = config["capital"].toObject();
        if (capital.contains("totalCapital")) setEquity(capital["totalCapital"].toDouble());
    }
}

void RiskManager::setMaxRiskPerTrade(double percent)
{
    QMutexLocker locker(&m_mutex);
    m_maxRiskPerTrade = percent;
}

void RiskManager::setMaxDailyRisk(double percent)
{
    QMutexLocker locker(&m_mutex);
    m_maxDailyRisk = percent;
}

void RiskManager::setMaxOpenPositions(int count)
{
    QMutexLocker locker(&m_mutex);
    m_maxOpenPositions = count;
}

void RiskManager::setMaxDrawdown(double percent)
{
    QMutexLocker locker(&m_mutex);
    m_maxDrawdownPercent = percent;
}

void RiskManager::setEquity(double equity)
{
    QMutexLocker locker(&m_mutex);
    m_equity = equity;
}

void RiskManager::setMaxTradesPerDay(int count)
{
    QMutexLocker locker(&m_mutex);
    m_maxTradesPerDay = count;
}

void RiskManager::setCounterTradingEnabled(bool enabled)
{
    QMutexLocker locker(&m_mutex);
    m_counterTradingEnabled = enabled;
}

void RiskManager::setTradesPerCounter(int count)
{
    QMutexLocker locker(&m_mutex);
    m_tradesPerCounter = count;
}

double RiskManager::calculateLotSize(const QString &symbol, double stopLossPips, double riskPercent)
{
    QMutexLocker locker(&m_mutex);
    // Standard risk: Lot size = (Equity * Risk%) / (StopLoss in price units)
    double riskAmount = m_equity * (riskPercent / 100.0);
    if (stopLossPips <= 0.0) return 0.01;
    double lotSize = riskAmount / stopLossPips;
    if (lotSize < 0.01) lotSize = 0.01;
    double maxLot = calculateMaxLotSize(symbol);
    if (lotSize > maxLot) lotSize = maxLot;
    return lotSize;
}

double RiskManager::calculateMaxLotSize(const QString &symbol)
{
    Q_UNUSED(symbol)
    
    // Stub implementation
    return 1.0;
}

bool RiskManager::shouldClosePosition(const QString &positionId)
{
    QMutexLocker locker(&m_mutex);
    // Close if drawdown or daily risk exceeded
    if (isDailyRiskExceeded() || isDrawdownExceeded()) return true;
    // Optionally, close if unrealized loss exceeds risk per trade
    auto it = m_positionMap.find(positionId);
    if (it != m_positionMap.end()) {
        const Position &pos = it->second;
        double maxRisk = m_equity * (m_maxRiskPerTrade / 100.0);
        if (std::abs(pos.unrealizedPnL) > maxRisk) return true;
    }
    return false;
}

bool RiskManager::isRiskAcceptable(double lotSize, double stopLossPips)
{
    QMutexLocker locker(&m_mutex);
    // Check if this trade would exceed max risk per trade or daily risk
    double riskAmount = lotSize * stopLossPips;
    double maxRisk = m_equity * (m_maxRiskPerTrade / 100.0);
    if (riskAmount > maxRisk) return false;
    double dailyRiskLimit = m_equity * (m_maxDailyRisk / 100.0);
    if ((std::abs(m_dailyPnL) + riskAmount) > dailyRiskLimit) return false;
    return true;
}

bool RiskManager::canOpenPosition(const QString &symbol, double lotSize)
{
    QMutexLocker locker(&m_mutex);
    // Check open positions
    if (m_openPositions.size() >= static_cast<size_t>(m_maxOpenPositions)) return false;
    // Check daily trade count
    if (m_dailyTradeCount >= m_maxTradesPerDay) return false;
    // Check daily risk
    double dailyRiskLimit = m_equity * (m_maxDailyRisk / 100.0);
    if (std::abs(m_dailyPnL) >= dailyRiskLimit) return false;
    // Check drawdown
    double drawdownLimit = m_initialEquity * (m_maxDrawdownPercent / 100.0);
    if (m_maxDrawdown >= drawdownLimit) return false;
    // Check risk per trade
    double maxRisk = m_equity * (m_maxRiskPerTrade / 100.0);
    if ((lotSize * 10.0) > maxRisk) return false; // Assume 10 price units as default stop loss if not provided
    // Counter trading logic
    if (m_counterTradingEnabled && m_currentCounterTrades >= m_tradesPerCounter) return false;
    return true;
}

bool RiskManager::isDailyRiskExceeded()
{
    QMutexLocker locker(&m_mutex);
    double dailyRiskLimit = m_equity * (m_maxDailyRisk / 100.0);
    return std::abs(m_dailyPnL) >= dailyRiskLimit;
}

bool RiskManager::isDrawdownExceeded()
{
    QMutexLocker locker(&m_mutex);
    double drawdownLimit = m_initialEquity * (m_maxDrawdownPercent / 100.0);
    return m_maxDrawdown >= drawdownLimit;
}

bool RiskManager::isMaxTradesReached()
{
    QMutexLocker locker(&m_mutex);
    return m_dailyTradeCount >= m_maxTradesPerDay;
}

void RiskManager::addPosition(const Position &position)
{
    QMutexLocker locker(&m_mutex);
    m_openPositions.push_back(position);
    m_positionMap[position.orderId] = position;
    emit positionOpened(position);
}

void RiskManager::updatePosition(const QString &positionId, double currentPrice)
{
    QMutexLocker locker(&m_mutex);
    
    auto it = m_positionMap.find(positionId);
    if (it != m_positionMap.end()) {
        it->second.currentPrice = currentPrice;
        // Update unrealized P&L
        it->second.unrealizedPnL = calculateUnrealizedPnL(it->second);
        emit positionUpdated(it->second);
    }
}

void RiskManager::closePosition(const QString &positionId, double closePrice)
{
    QMutexLocker locker(&m_mutex);
    
    auto it = m_positionMap.find(positionId);
    if (it != m_positionMap.end()) {
        Position &position = it->second;
        position.currentPrice = closePrice;
        position.closeTime = QDateTime::currentDateTime();
        position.isOpen = false;
        
        // Calculate realized P&L
        double pnl = calculateUnrealizedPnL(position);
        position.realizedPnL = pnl;
        
        // Update statistics
        m_dailyPnL += pnl;
        m_totalTrades++;
        m_dailyTradeCount++;
        
        if (pnl > 0) {
            m_winningTrades++;
            m_totalProfit += pnl;
            if (pnl > m_largestWin) {
                m_largestWin = pnl;
            }
        } else {
            m_totalLoss += std::abs(pnl);
            if (std::abs(pnl) > m_largestLoss) {
                m_largestLoss = std::abs(pnl);
            }
        }
        
        // Move to closed positions
        m_closedPositions.push_back(position);
        
        // Remove from open positions
        m_openPositions.erase(
            std::remove_if(m_openPositions.begin(), m_openPositions.end(),
                [&positionId](const Position &p) { return p.orderId == positionId; }),
            m_openPositions.end()
        );
        
        emit positionClosed(position);
    }
}

void RiskManager::clearAllPositions()
{
    QMutexLocker locker(&m_mutex);
    m_openPositions.clear();
    m_positionMap.clear();
}

void RiskManager::startNewCounter()
{
    QMutexLocker locker(&m_mutex);
    m_counterNumber++;
    m_currentCounterTrades = 0;
    m_counterStartEquity = m_equity;
}

void RiskManager::endCurrentCounter()
{
    QMutexLocker locker(&m_mutex);
    double counterPnL = m_equity - m_counterStartEquity;
    m_counterResults.push_back(counterPnL);
    emit counterCompleted(m_counterNumber, counterPnL);
}

bool RiskManager::isCounterComplete()
{
    QMutexLocker locker(&m_mutex);
    return m_currentCounterTrades >= m_tradesPerCounter;
}

bool RiskManager::shouldContinueTrading()
{
    QMutexLocker locker(&m_mutex);
    
    // Check if counter trading is enabled and counter is complete
    if (m_counterTradingEnabled && isCounterComplete()) {
        double counterPnL = m_equity - m_counterStartEquity;
        return counterPnL > 0; // Continue only if counter was profitable
    }
    
    return true; // Default to continue
}

RiskMetrics RiskManager::getRiskMetrics() const
{
    QMutexLocker locker(&m_mutex);
    
    RiskMetrics metrics;
    metrics.totalEquity = m_equity;
    metrics.availableMargin = m_equity * 0.8; // Assume 80% available
    metrics.usedMargin = m_equity * 0.2; // Assume 20% used
    metrics.dailyPnL = m_dailyPnL;
    metrics.weeklyPnL = m_weeklyPnL;
    metrics.monthlyPnL = m_monthlyPnL;
    metrics.maxDrawdown = m_maxDrawdown;
    metrics.riskUsed = m_riskUsed;
    metrics.riskRemaining = 100.0 - m_riskUsed;
    metrics.openPositions = static_cast<int>(m_openPositions.size());
    metrics.dailyTrades = m_dailyTradeCount;
    metrics.winRate = m_totalTrades > 0 ? (double)m_winningTrades / m_totalTrades * 100.0 : 0.0;
    metrics.profitFactor = m_totalLoss > 0 ? m_totalProfit / m_totalLoss : 0.0;
    metrics.sharpeRatio = 0.0; // Stub
    metrics.lastUpdate = QDateTime::currentDateTime();
    
    return metrics;
}

std::vector<Position> RiskManager::getOpenPositions() const
{
    QMutexLocker locker(&m_mutex);
    return m_openPositions;
}

std::vector<Position> RiskManager::getClosedPositions() const
{
    QMutexLocker locker(&m_mutex);
    return m_closedPositions;
}

Position RiskManager::getPosition(const QString &positionId) const
{
    QMutexLocker locker(&m_mutex);
    
    auto it = m_positionMap.find(positionId);
    if (it != m_positionMap.end()) {
        return it->second;
    }
    
    return Position(); // Return empty position if not found
}

void RiskManager::onUpdateRiskMetrics()
{
    calculateRiskMetrics();
}

void RiskManager::onCheckRiskLimits()
{
    checkRiskLimits();
}

void RiskManager::onUpdatePositions()
{
    // Update all open positions
    for (auto &position : m_openPositions) {
        position.unrealizedPnL = calculateUnrealizedPnL(position);
    }
}

void RiskManager::calculateRiskMetrics()
{
    QMutexLocker locker(&m_mutex);
    
    // Calculate current drawdown
    updateDrawdown();
    
    // Calculate risk usage
    double totalRisk = 0.0;
    for (const auto &position : m_openPositions) {
        totalRisk += calculatePositionRisk(position);
    }
    
    m_riskUsed = (totalRisk / m_equity) * 100.0;
    
    // Emit updated metrics
    emit riskMetricsUpdated(getRiskMetrics());
}

void RiskManager::updateDailyStatistics()
{
    // This would be called at the end of each trading day
    // Reset daily counters, move to weekly/monthly stats, etc.
}

void RiskManager::updateDrawdown()
{
    double currentDrawdown = m_initialEquity - m_equity;
    if (currentDrawdown > m_maxDrawdown) {
        m_maxDrawdown = currentDrawdown;
        emit drawdownWarning(currentDrawdown);
    }
}

void RiskManager::checkRiskLimits()
{
    if (isDailyRiskExceeded()) {
        emit riskLimitReached("Daily risk limit exceeded");
        emit tradingHalted("Daily risk limit exceeded");
    }
    
    if (isDrawdownExceeded()) {
        emit riskLimitReached("Maximum drawdown exceeded");
        emit tradingHalted("Maximum drawdown exceeded");
    }
    
    if (isMaxTradesReached()) {
        emit riskLimitReached("Maximum daily trades reached");
        emit tradingHalted("Maximum daily trades reached");
    }
}

void RiskManager::resetDailyCounters()
{
    QMutexLocker locker(&m_mutex);
    m_dailyTradeCount = 0;
    m_dailyPnL = 0.0;
    m_lastTradingDay = QDateTime::currentDateTime();
}

bool RiskManager::isNewTradingDay()
{
    QDateTime now = QDateTime::currentDateTime();
    return m_lastTradingDay.date() != now.date();
}

double RiskManager::calculateUnrealizedPnL(const Position &position) const
{
    // Simplified P&L calculation
    double priceDiff = position.currentPrice - position.entryPrice;
    if (position.side == "SELL") {
        priceDiff = -priceDiff;
    }
    return priceDiff * position.size;
}

double RiskManager::calculatePositionRisk(const Position &position) const
{
    // Calculate the risk for this position
    double stopLossDistance = std::abs(position.entryPrice - position.stopLoss);
    return stopLossDistance * position.size;
}

double RiskManager::getPipValue(const QString &symbol) const
{
    Q_UNUSED(symbol)
    return 1.0; // Stub implementation
}

double RiskManager::getMarginRequirement(const QString &symbol, double lotSize) const
{
    Q_UNUSED(symbol)
    Q_UNUSED(lotSize)
    return 100.0; // Stub implementation
} 