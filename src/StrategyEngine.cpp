#include "StrategyEngine.h"
#include <QDebug>
#include <QJsonObject>

StrategyEngine::StrategyEngine(QObject *parent)
    : QObject(parent)
    , m_symbol("BTCUSD")
    , m_brickSize(10.0)
    , m_setup1Enabled(true)
    , m_setup2Enabled(true)
    , m_tickBuffer(2)
    , m_riskPercent(2.0)
    , m_running(false)
    , m_paused(false)
    , m_initialized(false)
    , m_currentPrice(0.0)
    , m_inPattern(false)
    , m_patternBrickCount(0)
    , m_patternHighClose(0.0)
    , m_patternLowOpen(0.0)
    , m_totalSignals(0)
    , m_successfulSignals(0)
{
    initializeEngine();
}

StrategyEngine::~StrategyEngine()
{
    stop();
}

void StrategyEngine::initializeEngine()
{
    m_initialized = true;
    emit strategyStatusChanged("Engine initialized");
}

void StrategyEngine::start()
{
    if (!m_initialized) {
        emit errorOccurred("Engine not initialized");
        return;
    }
    
    m_running = true;
    m_paused = false;
    emit strategyStatusChanged("Engine started");
}

void StrategyEngine::stop()
{
    m_running = false;
    m_paused = false;
    emit strategyStatusChanged("Engine stopped");
}

void StrategyEngine::pause()
{
    if (m_running) {
        m_paused = true;
        emit strategyStatusChanged("Engine paused");
    }
}

void StrategyEngine::resume()
{
    if (m_running && m_paused) {
        m_paused = false;
        emit strategyStatusChanged("Engine resumed");
    }
}

void StrategyEngine::setSymbol(const QString &symbol)
{
    m_symbol = symbol;
}

void StrategyEngine::setBrickSize(double size)
{
    if (size >= MIN_BRICK_SIZE && size <= MAX_BRICK_SIZE) {
        m_brickSize = size;
    }
}

void StrategyEngine::setSetup1Enabled(bool enabled)
{
    m_setup1Enabled = enabled;
}

void StrategyEngine::setSetup2Enabled(bool enabled)
{
    m_setup2Enabled = enabled;
}

void StrategyEngine::setTickBuffer(int buffer)
{
    m_tickBuffer = buffer;
}

void StrategyEngine::setRiskPercent(double percent)
{
    m_riskPercent = percent;
}

std::vector<RenkoBrick> StrategyEngine::getRenkoBricks() const
{
    QMutexLocker locker(&m_mutex);
    return m_renkoBricks;
}

double StrategyEngine::getWinRate() const
{
    if (m_totalSignals == 0) return 0.0;
    return (double)m_successfulSignals / m_totalSignals * 100.0;
}

void StrategyEngine::onProcessTick()
{
    // Stub implementation
}

void StrategyEngine::onAnalyzePattern()
{
    // Stub implementation
}

void StrategyEngine::onValidateSignal()
{
    // Stub implementation
}

void StrategyEngine::processPriceData(double price, const QDateTime &timestamp)
{
    QMutexLocker locker(&m_mutex);
    m_currentPrice = price;
    formRenkoBrick(price, timestamp);
}

void StrategyEngine::formRenkoBrick(double price, const QDateTime &timestamp)
{
    if (m_renkoBricks.empty()) {
        RenkoBrick firstBrick;
        firstBrick.open = price;
        firstBrick.close = price;
        firstBrick.high = price;
        firstBrick.low = price;
        firstBrick.timestamp = timestamp;
        firstBrick.isGreen = false;
        firstBrick.isRed = false;
        firstBrick.formationPercentage = 0.0;
        m_renkoBricks.push_back(firstBrick);
        emit brickFormed(firstBrick);
        return;
    }
    RenkoBrick &lastBrick = m_renkoBricks.back();
    double diff = price - lastBrick.close;
    int bricksToForm = static_cast<int>(std::abs(diff) / m_brickSize);
    if (bricksToForm == 0) return;
    for (int i = 0; i < bricksToForm; ++i) {
        RenkoBrick newBrick;
        newBrick.open = lastBrick.close;
        if (diff > 0) {
            newBrick.close = lastBrick.close + m_brickSize;
            newBrick.isGreen = true;
            newBrick.isRed = false;
        } else {
            newBrick.close = lastBrick.close - m_brickSize;
            newBrick.isGreen = false;
            newBrick.isRed = true;
        }
        newBrick.high = std::max(newBrick.open, newBrick.close);
        newBrick.low = std::min(newBrick.open, newBrick.close);
        newBrick.timestamp = timestamp;
        newBrick.formationPercentage = 1.0;
        m_renkoBricks.push_back(newBrick);
        if (m_renkoBricks.size() > MAX_BRICK_HISTORY) {
            m_renkoBricks.erase(m_renkoBricks.begin());
        }
        emit brickFormed(newBrick);
        analyzeRenkoPattern();
    }
}

void StrategyEngine::analyzeRenkoPattern()
{
    TradingSignal signal;
    if (m_setup1Enabled && detectTwoRedOneGreen()) {
        signal = analyzeSetup1();
    } else if (m_setup2Enabled && detectThreeBrickPattern()) {
        signal = analyzeSetup2();
    } else {
        return;
    }
    if (signal.isValid && validateSignal(signal)) {
        m_lastSignal = signal;
        m_signalHistory.push_back(signal);
        if (m_signalHistory.size() > MAX_SIGNAL_HISTORY) {
            m_signalHistory.erase(m_signalHistory.begin());
        }
        ++m_totalSignals;
        emit newSignal(signal);
        logSignal(signal);
    }
}

bool StrategyEngine::detectTwoRedOneGreen()
{
    if (m_renkoBricks.size() < 3) return false;
    const RenkoBrick &b1 = m_renkoBricks[m_renkoBricks.size() - 3];
    const RenkoBrick &b2 = m_renkoBricks[m_renkoBricks.size() - 2];
    const RenkoBrick &b3 = m_renkoBricks[m_renkoBricks.size() - 1];
    return b1.isRed && b2.isRed && b3.isGreen;
}

bool StrategyEngine::detectThreeBrickPattern()
{
    if (m_renkoBricks.size() < 3) return false;
    // Example: three consecutive green bricks
    const RenkoBrick &b1 = m_renkoBricks[m_renkoBricks.size() - 3];
    const RenkoBrick &b2 = m_renkoBricks[m_renkoBricks.size() - 2];
    const RenkoBrick &b3 = m_renkoBricks[m_renkoBricks.size() - 1];
    return b1.isGreen && b2.isGreen && b3.isGreen;
}

TradingSignal StrategyEngine::analyzeSetup1()
{
    TradingSignal signal;
    signal.setup = TradingSignal::SETUP1;
    signal.symbol = m_symbol;
    signal.timestamp = QDateTime::currentDateTime();
    signal.type = TradingSignal::BUY;
    signal.price = m_renkoBricks.back().close;
    signal.lotSize = calculateLotSize(signal);
    signal.stopLoss = calculateStopLoss(signal);
    signal.takeProfit = calculateTakeProfit(signal);
    signal.isValid = true;
    signal.description = "Setup1: Two red, one green pattern detected.";
    return signal;
}

TradingSignal StrategyEngine::analyzeSetup2()
{
    TradingSignal signal;
    signal.setup = TradingSignal::SETUP2;
    signal.symbol = m_symbol;
    signal.timestamp = QDateTime::currentDateTime();
    signal.type = TradingSignal::BUY;
    signal.price = m_renkoBricks.back().close;
    signal.lotSize = calculateLotSize(signal);
    signal.stopLoss = calculateStopLoss(signal);
    signal.takeProfit = calculateTakeProfit(signal);
    signal.isValid = true;
    signal.description = "Setup2: Three green bricks pattern detected.";
    return signal;
}

bool StrategyEngine::validateSignal(const TradingSignal &signal)
{
    Q_UNUSED(signal)
    return true;
}

double StrategyEngine::calculateLotSize(const TradingSignal &signal)
{
    // Standard risk management: risk a fixed percent of account per trade
    // Lot size = (AccountSize * RiskPercent) / (StopLoss in price units)
    double accountSize = 10000.0; // Example fixed account size, should be loaded from config
    double riskAmount = accountSize * (m_riskPercent / 100.0);
    double stopLoss = calculateStopLoss(signal);
    if (stopLoss <= 0.0) return 0.01; // fallback
    double lotSize = riskAmount / stopLoss;
    if (lotSize < 0.01) lotSize = 0.01;
    return lotSize;
}

double StrategyEngine::calculateStopLoss(const TradingSignal &signal)
{
    // Standard: stop loss = brick size * 2 (for Renko reversal)
    return m_brickSize * 2;
}

double StrategyEngine::calculateTakeProfit(const TradingSignal &signal)
{
    // Standard: take profit = brick size * 3 (risk:reward 1:1.5)
    return m_brickSize * 3;
}

bool StrategyEngine::isGreenBrick(const RenkoBrick &brick) const
{
    return brick.isGreen;
}

bool StrategyEngine::isRedBrick(const RenkoBrick &brick) const
{
    return brick.isRed;
}

double StrategyEngine::getBrickFormationPercentage(double currentPrice, double brickOpen) const
{
    Q_UNUSED(currentPrice)
    Q_UNUSED(brickOpen)
    return 0.0;
}

void StrategyEngine::updatePatternHistory()
{
    // Stub implementation
}

void StrategyEngine::logSignal(const TradingSignal &signal)
{
    Q_UNUSED(signal)
    // Stub implementation
}

void StrategyEngine::loadConfig(const QJsonObject &config)
{
    if (config.contains("strategy")) {
        QJsonObject strat = config["strategy"].toObject();
        if (strat.contains("brickSize")) setBrickSize(strat["brickSize"].toDouble());
        if (strat.contains("tickBuffer")) setTickBuffer(strat["tickBuffer"].toInt());
        if (strat.contains("setup1Enabled")) setSetup1Enabled(strat["setup1Enabled"].toBool());
        if (strat.contains("setup2Enabled")) setSetup2Enabled(strat["setup2Enabled"].toBool());
    }
    if (config.contains("trading")) {
        QJsonObject trading = config["trading"].toObject();
        if (trading.contains("defaultSymbol")) setSymbol(trading["defaultSymbol"].toString());
    }
    if (config.contains("risk")) {
        QJsonObject risk = config["risk"].toObject();
        if (risk.contains("maxRiskPerTrade")) setRiskPercent(risk["maxRiskPerTrade"].toDouble());
    }
} 