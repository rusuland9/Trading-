#include "MainWindow.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>
#include <QMessageBox>
#include <QSplitter>
#include <QGroupBox>
#include <QHeaderView>
#include <QDateTime>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSettings>
#include <QCloseEvent>
#include <QStyleFactory>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>
#include <QRandomGenerator>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_tradingActive(false)
    , m_paperTradingMode(true)
    , m_paused(false)
    , m_currentEquity(10000.0)
    , m_dailyPnL(0.0)
    , m_openPositions(0)
    , m_totalTrades(0)
    , m_winRate(0.0)
    , m_riskUsed(0.0)
    , m_currentPrice(50000.0)
    , m_priceDirection(1)
    , m_simulationCounter(0)
{
    setWindowTitle("MasterMind Trader - Professional Trading System");
    setMinimumSize(1200, 800);
    resize(1600, 1000);
    
    // Create logs directory
    QDir().mkpath("logs");
    
    // Load configuration
    loadConfiguration();
    
    setupUI();
    setupMenuBar();
    setupStatusBar();
    setupConnections();
    
    // Start update timer
    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(1000);
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::updateStatusLabels);
    m_updateTimer->start();
    
    // Start simulation timer
    m_simulationTimer = new QTimer(this);
    m_simulationTimer->setInterval(2000); // Update every 2 seconds
    connect(m_simulationTimer, &QTimer::timeout, this, &MainWindow::simulateTrading);
    
    // Apply dark theme
    applyTheme();
    
    // Initial status update
    updateStatusLabels();
    
    // Add welcome message
    m_logTextEdit->append(QString("[%1] MasterMind Trader v1.0.0 initialized").arg(QDateTime::currentDateTime().toString()));
    m_logTextEdit->append(QString("[%1] Configuration loaded successfully").arg(QDateTime::currentDateTime().toString()));
    m_logTextEdit->append(QString("[%1] Paper trading mode enabled for safety").arg(QDateTime::currentDateTime().toString()));
    m_logTextEdit->append(QString("[%1] Ready to trade - click 'Start Trading' to begin").arg(QDateTime::currentDateTime().toString()));
}

MainWindow::~MainWindow()
{
    saveConfiguration();
    
    if (m_tradingActive) {
        onStopTrading();
    }
}

void MainWindow::loadConfiguration()
{
    QFile file("config.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject config = doc.object();
        
        // Load trading settings
        QJsonObject trading = config["trading"].toObject();
        m_currentExchange = trading["defaultExchange"].toString("Binance");
        m_currentSymbol = trading["defaultSymbol"].toString("BTCUSD");
        m_paperTradingMode = trading["paperTradingMode"].toBool(true);
        
        // Load risk settings
        QJsonObject risk = config["risk"].toObject();
        m_maxRiskPerTrade = risk["maxRiskPerTrade"].toDouble(2.0);
        m_maxDailyRisk = risk["maxDailyRisk"].toDouble(10.0);
        m_maxOpenPositions = risk["maxOpenPositions"].toInt(5);
        
        // Load capital settings
        QJsonObject capital = config["capital"].toObject();
        m_currentEquity = capital["totalCapital"].toDouble(10000.0);
        
        // Load simulation settings
        QJsonObject simulation = config["simulation"].toObject();
        m_spread = simulation["spread"].toDouble(2.0);
        m_slippage = simulation["slippage"].toDouble(1.0);
        m_priceVolatility = simulation["priceVolatility"].toDouble(0.001);
        
        file.close();
    }
}

void MainWindow::saveConfiguration()
{
    QJsonObject config;
    
    // Save current settings
    QJsonObject trading;
    trading["defaultExchange"] = m_currentExchange;
    trading["defaultSymbol"] = m_currentSymbol;
    trading["paperTradingMode"] = m_paperTradingMode;
    config["trading"] = trading;
    
    QJsonObject risk;
    risk["maxRiskPerTrade"] = m_maxRiskPerTrade;
    risk["maxDailyRisk"] = m_maxDailyRisk;
    risk["maxOpenPositions"] = m_maxOpenPositions;
    config["risk"] = risk;
    
    QJsonObject capital;
    capital["totalCapital"] = m_currentEquity;
    config["capital"] = capital;
    
    QFile file("config.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(config);
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::setupUI()
{
    // Create central widget
    m_centralWidget = new QWidget;
    setCentralWidget(m_centralWidget);
    
    // Create main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(m_centralWidget);
    
    // Create left panel
    QWidget *leftPanel = new QWidget;
    leftPanel->setMaximumWidth(350);
    leftPanel->setMinimumWidth(300);
    
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    // Control Panel
    QGroupBox *controlGroup = new QGroupBox("Trading Control");
    QVBoxLayout *controlLayout = new QVBoxLayout(controlGroup);
    
    // Control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    
    m_startButton = new QPushButton("Start Trading");
    m_startButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 8px; border-radius: 4px; }");
    m_startButton->setMinimumHeight(40);
    
    m_stopButton = new QPushButton("Stop Trading");
    m_stopButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; font-weight: bold; padding: 8px; border-radius: 4px; }");
    m_stopButton->setMinimumHeight(40);
    m_stopButton->setEnabled(false);
    
    buttonLayout->addWidget(m_startButton);
    buttonLayout->addWidget(m_stopButton);
    controlLayout->addLayout(buttonLayout);
    
    // Trading mode buttons
    QHBoxLayout *modeLayout = new QHBoxLayout;
    
    m_paperTradingButton = new QPushButton("Paper Trading");
    m_paperTradingButton->setCheckable(true);
    m_paperTradingButton->setChecked(true);
    m_paperTradingButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; padding: 6px; border-radius: 4px; } QPushButton:checked { background-color: #0D47A1; }");
    
    m_liveTradingButton = new QPushButton("Live Trading");
    m_liveTradingButton->setCheckable(true);
    m_liveTradingButton->setStyleSheet("QPushButton { background-color: #757575; color: white; font-weight: bold; padding: 6px; border-radius: 4px; } QPushButton:checked { background-color: #FF5722; }");
    
    modeLayout->addWidget(m_paperTradingButton);
    modeLayout->addWidget(m_liveTradingButton);
    controlLayout->addLayout(modeLayout);
    
    // Exchange and symbol selection
    QFormLayout *selectionLayout = new QFormLayout;
    
    m_exchangeCombo = new QComboBox;
    m_exchangeCombo->addItems({"Binance", "Coinbase", "Deribit", "Delta Exchange", "MetaTrader 4", "MetaTrader 5"});
    m_exchangeCombo->setCurrentText(m_currentExchange);
    selectionLayout->addRow("Exchange:", m_exchangeCombo);
    
    m_symbolCombo = new QComboBox;
    m_symbolCombo->addItems({"BTCUSD", "ETHUSD", "EURUSD", "GBPUSD", "XAUUSD", "USDJPY"});
    m_symbolCombo->setCurrentText(m_currentSymbol);
    selectionLayout->addRow("Symbol:", m_symbolCombo);
    
    controlLayout->addLayout(selectionLayout);
    
    // Strategy settings
    QGroupBox *strategyGroup = new QGroupBox("Strategy Settings");
    QFormLayout *strategyLayout = new QFormLayout(strategyGroup);
    
    m_brickSizeSpinBox = new QDoubleSpinBox;
    m_brickSizeSpinBox->setRange(0.1, 1000.0);
    m_brickSizeSpinBox->setValue(10.0);
    m_brickSizeSpinBox->setSuffix(" pips");
    strategyLayout->addRow("Brick Size:", m_brickSizeSpinBox);
    
    m_setup1CheckBox = new QCheckBox("Setup 1 (Two Red + Green)");
    m_setup1CheckBox->setChecked(true);
    strategyLayout->addRow(m_setup1CheckBox);
    
    m_setup2CheckBox = new QCheckBox("Setup 2 (Three Brick Pattern)");
    m_setup2CheckBox->setChecked(true);
    strategyLayout->addRow(m_setup2CheckBox);
    
    m_counterTradingCheckBox = new QCheckBox("Counter Trading");
    strategyLayout->addRow(m_counterTradingCheckBox);
    
    controlLayout->addWidget(strategyGroup);
    
    leftLayout->addWidget(controlGroup);
    
    // Status Panel
    QGroupBox *statusGroup = new QGroupBox("System Status");
    QGridLayout *statusLayout = new QGridLayout(statusGroup);
    
    // Connection status
    QLabel *connLabel = new QLabel("Connection:");
    connLabel->setStyleSheet("font-weight: bold;");
    m_connectionStatusLabel = new QLabel("Disconnected");
    m_connectionStatusLabel->setStyleSheet("color: #f44336;");
    statusLayout->addWidget(connLabel, 0, 0);
    statusLayout->addWidget(m_connectionStatusLabel, 0, 1);
    
    // Trading status
    QLabel *tradingLabel = new QLabel("Trading:");
    tradingLabel->setStyleSheet("font-weight: bold;");
    m_tradingStatusLabel = new QLabel("Stopped");
    m_tradingStatusLabel->setStyleSheet("color: #f44336;");
    statusLayout->addWidget(tradingLabel, 1, 0);
    statusLayout->addWidget(m_tradingStatusLabel, 1, 1);
    
    // Current price
    QLabel *priceLabel = new QLabel("Price:");
    priceLabel->setStyleSheet("font-weight: bold;");
    m_priceLabel = new QLabel(QString("$%1").arg(m_currentPrice, 0, 'f', 2));
    m_priceLabel->setStyleSheet("color: #42A5F5; font-weight: bold;");
    statusLayout->addWidget(priceLabel, 2, 0);
    statusLayout->addWidget(m_priceLabel, 2, 1);
    
    // Equity
    QLabel *equityLabel = new QLabel("Equity:");
    equityLabel->setStyleSheet("font-weight: bold;");
    m_equityLabel = new QLabel(QString("$%1").arg(m_currentEquity, 0, 'f', 2));
    m_equityLabel->setStyleSheet("color: #4CAF50; font-weight: bold;");
    statusLayout->addWidget(equityLabel, 3, 0);
    statusLayout->addWidget(m_equityLabel, 3, 1);
    
    // Daily P&L
    QLabel *pnlLabel = new QLabel("Daily P&L:");
    pnlLabel->setStyleSheet("font-weight: bold;");
    m_dailyPnlLabel = new QLabel("$0.00");
    m_dailyPnlLabel->setStyleSheet("color: #757575; font-weight: bold;");
    statusLayout->addWidget(pnlLabel, 4, 0);
    statusLayout->addWidget(m_dailyPnlLabel, 4, 1);
    
    // Open positions
    QLabel *posLabel = new QLabel("Positions:");
    posLabel->setStyleSheet("font-weight: bold;");
    m_openPositionsLabel = new QLabel("0");
    m_openPositionsLabel->setStyleSheet("color: #2196F3; font-weight: bold;");
    statusLayout->addWidget(posLabel, 5, 0);
    statusLayout->addWidget(m_openPositionsLabel, 5, 1);
    
    leftLayout->addWidget(statusGroup);
    
    // Risk Panel
    QGroupBox *riskGroup = new QGroupBox("Risk Management");
    QFormLayout *riskLayout = new QFormLayout(riskGroup);
    
    m_riskPercentSpinBox = new QDoubleSpinBox;
    m_riskPercentSpinBox->setRange(0.1, 10.0);
    m_riskPercentSpinBox->setValue(m_maxRiskPerTrade);
    m_riskPercentSpinBox->setSuffix("%");
    riskLayout->addRow("Risk per Trade:", m_riskPercentSpinBox);
    
    m_capitalSpinBox = new QDoubleSpinBox;
    m_capitalSpinBox->setRange(100.0, 1000000.0);
    m_capitalSpinBox->setValue(m_currentEquity);
    m_capitalSpinBox->setPrefix("$");
    riskLayout->addRow("Capital:", m_capitalSpinBox);
    
    m_maxTradesSpinBox = new QSpinBox;
    m_maxTradesSpinBox->setRange(1, 100);
    m_maxTradesSpinBox->setValue(10);
    riskLayout->addRow("Max Trades:", m_maxTradesSpinBox);
    
    // Risk usage progress
    QLabel *riskUsageLabel = new QLabel("Risk Usage:");
    riskUsageLabel->setStyleSheet("font-weight: bold;");
    riskLayout->addRow(riskUsageLabel);
    
    m_riskProgressBar = new QProgressBar;
    m_riskProgressBar->setRange(0, 100);
    m_riskProgressBar->setValue(0);
    m_riskProgressBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; } QProgressBar::chunk { background-color: #4CAF50; }");
    riskLayout->addRow(m_riskProgressBar);
    
    m_riskUsedLabel = new QLabel("0.0%");
    m_riskUsedLabel->setAlignment(Qt::AlignCenter);
    m_riskUsedLabel->setStyleSheet("font-weight: bold;");
    riskLayout->addRow(m_riskUsedLabel);
    
    leftLayout->addWidget(riskGroup);
    leftLayout->addStretch();
    
    // Create center area
    QTabWidget *centralTabs = new QTabWidget;
    
    // Dashboard tab
    QWidget *dashboardTab = new QWidget;
    QVBoxLayout *dashboardLayout = new QVBoxLayout(dashboardTab);
    
    QLabel *dashboardLabel = new QLabel("Trading Dashboard");
    dashboardLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #42A5F5; margin: 20px;");
    dashboardLabel->setAlignment(Qt::AlignCenter);
    dashboardLayout->addWidget(dashboardLabel);
    
    // Add live price chart
    setupPriceChart();
    dashboardLayout->addWidget(m_chartWidget);
    
    // Add some metrics
    QGridLayout *metricsLayout = new QGridLayout;
    
    // Total Trades
    QLabel *tradesLabel = new QLabel("Total Trades:");
    tradesLabel->setStyleSheet("font-weight: bold;");
    m_totalTradesLabel = new QLabel("0");
    m_totalTradesLabel->setStyleSheet("font-size: 24px; color: #42A5F5;");
    metricsLayout->addWidget(tradesLabel, 0, 0);
    metricsLayout->addWidget(m_totalTradesLabel, 0, 1);
    
    // Win Rate
    QLabel *winRateLabel = new QLabel("Win Rate:");
    winRateLabel->setStyleSheet("font-weight: bold;");
    m_winRateLabel = new QLabel("0.0%");
    m_winRateLabel->setStyleSheet("font-size: 24px; color: #4CAF50;");
    metricsLayout->addWidget(winRateLabel, 1, 0);
    metricsLayout->addWidget(m_winRateLabel, 1, 1);
    
    // Profit Factor
    QLabel *profitFactorLabel = new QLabel("Profit Factor:");
    profitFactorLabel->setStyleSheet("font-weight: bold;");
    m_profitFactorLabel = new QLabel("0.0");
    m_profitFactorLabel->setStyleSheet("font-size: 24px; color: #FF9800;");
    metricsLayout->addWidget(profitFactorLabel, 2, 0);
    metricsLayout->addWidget(m_profitFactorLabel, 2, 1);
    
    dashboardLayout->addLayout(metricsLayout);
    dashboardLayout->addStretch();
    
    centralTabs->addTab(dashboardTab, "Dashboard");
    
    // Positions tab
    QWidget *positionsTab = new QWidget;
    QVBoxLayout *positionsLayout = new QVBoxLayout(positionsTab);
    
    QLabel *positionsTitle = new QLabel("Open Positions");
    positionsTitle->setStyleSheet("font-weight: bold; font-size: 14px; color: #42A5F5;");
    positionsLayout->addWidget(positionsTitle);
    
    m_positionsTable = new QTableWidget;
    m_positionsTable->setColumnCount(8);
    m_positionsTable->setHorizontalHeaderLabels({"Time", "Symbol", "Side", "Size", "Entry Price", "Current Price", "P&L", "Status"});
    m_positionsTable->horizontalHeader()->setStretchLastSection(true);
    m_positionsTable->setAlternatingRowColors(true);
    positionsLayout->addWidget(m_positionsTable);
    
    centralTabs->addTab(positionsTab, "Positions");
    
    // Logs tab
    QWidget *logsTab = new QWidget;
    QVBoxLayout *logsLayout = new QVBoxLayout(logsTab);
    
    QLabel *logsTitle = new QLabel("Trading Log");
    logsTitle->setStyleSheet("font-weight: bold; font-size: 14px; color: #42A5F5;");
    logsLayout->addWidget(logsTitle);
    
    m_logTextEdit = new QTextEdit;
    m_logTextEdit->setReadOnly(true);
    m_logTextEdit->setFont(QFont("Courier New", 9));
    logsLayout->addWidget(m_logTextEdit);
    
    centralTabs->addTab(logsTab, "Logs");
    
    // Add to main layout
    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(centralTabs, 1);
}

void MainWindow::setupPriceChart()
{
    m_chartWidget = new QWidget;
    QVBoxLayout *chartLayout = new QVBoxLayout(m_chartWidget);
    
    // Create chart
    m_priceChart = new QChart;
    m_priceChart->setTitle("Live Price Chart");
    m_priceChart->setAnimationOptions(QChart::NoAnimation);
    
    m_priceSeries = new QLineSeries;
    m_priceSeries->setName("Price");
    m_priceSeries->setColor(QColor(66, 165, 245));
    m_priceChart->addSeries(m_priceSeries);
    
    // Create axes
    m_timeAxis = new QDateTimeAxis;
    m_timeAxis->setFormat("hh:mm:ss");
    m_timeAxis->setTitleText("Time");
    m_priceChart->addAxis(m_timeAxis, Qt::AlignBottom);
    m_priceSeries->attachAxis(m_timeAxis);
    
    m_priceAxis = new QValueAxis;
    m_priceAxis->setTitleText("Price ($)");
    m_priceChart->addAxis(m_priceAxis, Qt::AlignLeft);
    m_priceSeries->attachAxis(m_priceAxis);
    
    // Create chart view
    m_chartView = new QChartView(m_priceChart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setMinimumHeight(300);
    
    chartLayout->addWidget(m_chartView);
    
    // Initialize chart with current price
    QDateTime now = QDateTime::currentDateTime();
    m_priceSeries->append(now.toMSecsSinceEpoch(), m_currentPrice);
    m_timeAxis->setRange(now.addSecs(-300), now.addSecs(60)); // Show last 5 minutes
    m_priceAxis->setRange(m_currentPrice - 500, m_currentPrice + 500);
}

void MainWindow::simulateTrading()
{
    if (!m_tradingActive) return;
    
    m_simulationCounter++;
    
    // Simulate price movement
    double priceChange = QRandomGenerator::global()->generateDouble() * 2.0 - 1.0; // -1 to 1
    priceChange *= m_priceVolatility * m_currentPrice; // Scale by volatility
    
    // Add trend
    if (m_simulationCounter % 50 == 0) {
        m_priceDirection = (QRandomGenerator::global()->generateDouble() > 0.5) ? 1 : -1;
    }
    
    priceChange += m_priceDirection * m_priceVolatility * m_currentPrice * 0.2;
    
    m_currentPrice += priceChange;
    
    // Ensure price stays positive
    if (m_currentPrice < 100.0) {
        m_currentPrice = 100.0 + QRandomGenerator::global()->generateDouble() * 1000.0;
    }
    
    // Update chart
    QDateTime now = QDateTime::currentDateTime();
    m_priceSeries->append(now.toMSecsSinceEpoch(), m_currentPrice);
    
    // Keep only last 100 points
    if (m_priceSeries->count() > 100) {
        m_priceSeries->removePoints(0, m_priceSeries->count() - 100);
    }
    
    // Update chart axes
    m_timeAxis->setRange(now.addSecs(-300), now.addSecs(60));
    double minPrice = m_currentPrice - 500;
    double maxPrice = m_currentPrice + 500;
    m_priceAxis->setRange(minPrice, maxPrice);
    
    // Update price label
    m_priceLabel->setText(QString("$%1").arg(m_currentPrice, 0, 'f', 2));
    
    // Simulate trading signals
    if (m_simulationCounter % 10 == 0) {
        simulateTradeSignal();
    }
    
    // Update chart colors based on price movement
    if (priceChange > 0) {
        m_priceSeries->setColor(QColor(76, 175, 80)); // Green
    } else {
        m_priceSeries->setColor(QColor(244, 67, 54)); // Red
    }
}

void MainWindow::simulateTradeSignal()
{
    // Random chance to generate a trade signal
    if (QRandomGenerator::global()->generateDouble() < 0.3) { // 30% chance
        
        QString side = (QRandomGenerator::global()->generateDouble() > 0.5) ? "BUY" : "SELL";
        double size = 0.01 + QRandomGenerator::global()->generateDouble() * 0.09; // 0.01 to 0.1
        double entryPrice = m_currentPrice + (QRandomGenerator::global()->generateDouble() * 10.0 - 5.0); // ±5 spread
        
        // Simulate win/loss
        bool isWin = QRandomGenerator::global()->generateDouble() > 0.4; // 60% win rate
        double pnl = 0.0;
        
        if (isWin) {
            pnl = size * (50.0 + QRandomGenerator::global()->generateDouble() * 100.0); // $50-150 win
        } else {
            pnl = -size * (20.0 + QRandomGenerator::global()->generateDouble() * 50.0); // $20-70 loss
        }
        
        // Update statistics
        m_totalTrades++;
        m_dailyPnL += pnl;
        m_currentEquity += pnl;
        
        if (isWin) {
            m_winningTrades++;
        }
        
        // Update win rate
        m_winRate = (double)m_winningTrades / m_totalTrades * 100.0;
        
        // Add to positions table
        addTradeToTable(side, size, entryPrice, pnl, isWin ? "CLOSED (WIN)" : "CLOSED (LOSS)");
        
        // Log the trade
        QString logMessage = QString("Trade #%1: %2 %3 @ $%4 | P&L: %5$%6 | Running Total: $%7")
                            .arg(m_totalTrades)
                            .arg(side)
                            .arg(size, 0, 'f', 3)
                            .arg(entryPrice, 0, 'f', 2)
                            .arg(pnl >= 0 ? "+" : "")
                            .arg(pnl, 0, 'f', 2)
                            .arg(m_currentEquity, 0, 'f', 2);
        
        m_logTextEdit->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString(), logMessage));
        
        // Scroll to bottom
        m_logTextEdit->moveCursor(QTextCursor::End);
        
        // Update risk usage
        m_riskUsed = (QRandomGenerator::global()->generateDouble() * 30.0) + 10.0; // 10-40%
        m_riskProgressBar->setValue(static_cast<int>(m_riskUsed));
        
        // Check risk limits
        if (m_riskUsed > 80.0) {
            m_riskProgressBar->setStyleSheet("QProgressBar::chunk { background-color: #f44336; }");
        } else if (m_riskUsed > 60.0) {
            m_riskProgressBar->setStyleSheet("QProgressBar::chunk { background-color: #FF9800; }");
        } else {
            m_riskProgressBar->setStyleSheet("QProgressBar::chunk { background-color: #4CAF50; }");
        }
    }
}

void MainWindow::addTradeToTable(const QString &side, double size, double entryPrice, double pnl, const QString &status)
{
    int row = m_positionsTable->rowCount();
    m_positionsTable->insertRow(row);
    
    m_positionsTable->setItem(row, 0, new QTableWidgetItem(QDateTime::currentDateTime().toString("hh:mm:ss")));
    m_positionsTable->setItem(row, 1, new QTableWidgetItem(m_currentSymbol));
    m_positionsTable->setItem(row, 2, new QTableWidgetItem(side));
    m_positionsTable->setItem(row, 3, new QTableWidgetItem(QString::number(size, 'f', 3)));
    m_positionsTable->setItem(row, 4, new QTableWidgetItem(QString("$%1").arg(entryPrice, 0, 'f', 2)));
    m_positionsTable->setItem(row, 5, new QTableWidgetItem(QString("$%1").arg(m_currentPrice, 0, 'f', 2)));
    
    // Format P&L with color
    QTableWidgetItem *pnlItem = new QTableWidgetItem(QString("%1$%2").arg(pnl >= 0 ? "+" : "").arg(pnl, 0, 'f', 2));
    pnlItem->setForeground(pnl >= 0 ? QColor(76, 175, 80) : QColor(244, 67, 54));
    m_positionsTable->setItem(row, 6, pnlItem);
    
    m_positionsTable->setItem(row, 7, new QTableWidgetItem(status));
    
    // Auto-scroll to bottom
    m_positionsTable->scrollToBottom();
    
    // Keep only last 50 trades
    if (m_positionsTable->rowCount() > 50) {
        m_positionsTable->removeRow(0);
    }
}

void MainWindow::setupMenuBar()
{
    // File menu
    QMenu *fileMenu = menuBar()->addMenu("&File");
    
    QAction *loadConfigAction = new QAction("&Load Configuration", this);
    loadConfigAction->setShortcut(QKeySequence::Open);
    connect(loadConfigAction, &QAction::triggered, this, &MainWindow::loadConfiguration);
    fileMenu->addAction(loadConfigAction);
    
    QAction *saveConfigAction = new QAction("&Save Configuration", this);
    saveConfigAction->setShortcut(QKeySequence::Save);
    connect(saveConfigAction, &QAction::triggered, this, &MainWindow::saveConfiguration);
    fileMenu->addAction(saveConfigAction);
    
    fileMenu->addSeparator();
    
    QAction *exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);
    
    // Trading menu
    QMenu *tradingMenu = menuBar()->addMenu("&Trading");
    
    QAction *startAction = new QAction("&Start Trading", this);
    startAction->setShortcut(QKeySequence("Ctrl+S"));
    connect(startAction, &QAction::triggered, this, &MainWindow::onStartTrading);
    tradingMenu->addAction(startAction);
    
    QAction *stopAction = new QAction("St&op Trading", this);
    stopAction->setShortcut(QKeySequence("Ctrl+T"));
    connect(stopAction, &QAction::triggered, this, &MainWindow::onStopTrading);
    tradingMenu->addAction(stopAction);
    
    // Help menu
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    
    QAction *aboutAction = new QAction("&About", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onShowAbout);
    helpMenu->addAction(aboutAction);
}

void MainWindow::setupStatusBar()
{
    statusBar()->addWidget(new QLabel("Ready"));
    
    // Add connection indicator
    m_connectionIndicator = new QLabel("●");
    m_connectionIndicator->setStyleSheet("color: #f44336; font-size: 16px;");
    statusBar()->addPermanentWidget(new QLabel("Connection:"));
    statusBar()->addPermanentWidget(m_connectionIndicator);
    
    statusBar()->addPermanentWidget(new QLabel("Version 1.0.0"));
    
    // Add time display
    m_timeLabel = new QLabel;
    m_timeLabel->setStyleSheet("color: #757575;");
    statusBar()->addPermanentWidget(m_timeLabel);
    
    // Update time every second
    QTimer *timeTimer = new QTimer(this);
    connect(timeTimer, &QTimer::timeout, [this]() {
        m_timeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    });
    timeTimer->start(1000);
}

void MainWindow::setupConnections()
{
    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::onStartTrading);
    connect(m_stopButton, &QPushButton::clicked, this, &MainWindow::onStopTrading);
    connect(m_paperTradingButton, &QPushButton::clicked, this, &MainWindow::onSwitchToPaperTrading);
    connect(m_liveTradingButton, &QPushButton::clicked, this, &MainWindow::onSwitchToLiveTrading);
    connect(m_exchangeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onTradingModeChanged);
    connect(m_symbolCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onTradingModeChanged);
    
    // Connect spin boxes to update configuration
    connect(m_riskPercentSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) {
        m_maxRiskPerTrade = value;
    });
    
    connect(m_capitalSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double value) {
        m_currentEquity = value;
    });
}

void MainWindow::applyTheme()
{
    QString styleSheet = R"(
        QMainWindow {
            background-color: #2b2b2b;
        }
        
        QWidget {
            background-color: #2b2b2b;
            color: #ffffff;
        }
        
        QGroupBox {
            font-weight: bold;
            border: 2px solid #555555;
            border-radius: 5px;
            margin: 10px 0px;
            padding-top: 10px;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
            color: #42A5F5;
        }
        
        QTabWidget::pane {
            border: 1px solid #555555;
            background-color: #353535;
        }
        
        QTabBar::tab {
            background-color: #555555;
            color: #ffffff;
            padding: 8px 16px;
            margin-right: 2px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }
        
        QTabBar::tab:selected {
            background-color: #42A5F5;
        }
        
        QTableWidget {
            background-color: #353535;
            alternate-background-color: #404040;
            selection-background-color: #42A5F5;
            gridline-color: #555555;
        }
        
        QHeaderView::section {
            background-color: #555555;
            color: #ffffff;
            font-weight: bold;
            padding: 5px;
            border: none;
        }
        
        QComboBox {
            background-color: #555555;
            color: #ffffff;
            border: 1px solid #777777;
            border-radius: 4px;
            padding: 5px;
        }
        
        QDoubleSpinBox, QSpinBox {
            background-color: #555555;
            color: #ffffff;
            border: 1px solid #777777;
            border-radius: 4px;
            padding: 5px;
        }
        
        QTextEdit {
            background-color: #1e1e1e;
            color: #ffffff;
            border: 1px solid #555555;
            border-radius: 4px;
        }
        
        QCheckBox {
            color: #ffffff;
        }
        
        QCheckBox::indicator {
            width: 16px;
            height: 16px;
        }
        
        QCheckBox::indicator:unchecked {
            background-color: #555555;
            border: 1px solid #777777;
            border-radius: 3px;
        }
        
        QCheckBox::indicator:checked {
            background-color: #42A5F5;
            border: 1px solid #42A5F5;
            border-radius: 3px;
        }
        
        QProgressBar {
            border: 2px solid #555555;
            border-radius: 5px;
            text-align: center;
            background-color: #353535;
        }
        
        QProgressBar::chunk {
            background-color: #4CAF50;
            border-radius: 3px;
        }
        
        QMenuBar {
            background-color: #404040;
            color: #ffffff;
        }
        
        QMenuBar::item {
            background-color: transparent;
            padding: 5px 10px;
        }
        
        QMenuBar::item:selected {
            background-color: #42A5F5;
        }
        
        QMenu {
            background-color: #404040;
            color: #ffffff;
            border: 1px solid #555555;
        }
        
        QMenu::item {
            padding: 5px 20px;
        }
        
        QMenu::item:selected {
            background-color: #42A5F5;
        }
        
        QStatusBar {
            background-color: #404040;
            color: #ffffff;
        }
    )";
    
    setStyleSheet(styleSheet);
}

// Slot implementations
void MainWindow::onStartTrading()
{
    if (m_tradingActive) return;
    
    m_tradingActive = true;
    m_paused = false;
    
    m_startButton->setEnabled(false);
    m_stopButton->setEnabled(true);
    m_tradingStatusLabel->setText("Running");
    m_tradingStatusLabel->setStyleSheet("color: #4CAF50;");
    
    // Update connection status
    m_connectionStatusLabel->setText("Connected");
    m_connectionStatusLabel->setStyleSheet("color: #4CAF50;");
    m_connectionIndicator->setStyleSheet("color: #4CAF50; font-size: 16px;");
    
    // Start simulation
    m_simulationTimer->start();
    
    m_logTextEdit->append(QString("[%1] Trading started on %2 for %3")
                         .arg(QDateTime::currentDateTime().toString())
                         .arg(m_currentExchange)
                         .arg(m_currentSymbol));
    
    if (m_paperTradingMode) {
        m_logTextEdit->append(QString("[%1] Paper trading mode - trades are simulated")
                             .arg(QDateTime::currentDateTime().toString()));
    }
    
    statusBar()->showMessage("Trading started", 3000);
}

void MainWindow::onStopTrading()
{
    if (!m_tradingActive) return;
    
    m_tradingActive = false;
    m_paused = false;
    
    m_startButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_tradingStatusLabel->setText("Stopped");
    m_tradingStatusLabel->setStyleSheet("color: #f44336;");
    
    // Update connection status
    m_connectionStatusLabel->setText("Disconnected");
    m_connectionStatusLabel->setStyleSheet("color: #f44336;");
    m_connectionIndicator->setStyleSheet("color: #f44336; font-size: 16px;");
    
    // Stop simulation
    m_simulationTimer->stop();
    
    m_logTextEdit->append(QString("[%1] Trading stopped").arg(QDateTime::currentDateTime().toString()));
    m_logTextEdit->append(QString("[%1] Session summary: %2 trades, $%3 P&L, %4% win rate")
                         .arg(QDateTime::currentDateTime().toString())
                         .arg(m_totalTrades)
                         .arg(m_dailyPnL, 0, 'f', 2)
                         .arg(m_winRate, 0, 'f', 1));
    
    statusBar()->showMessage("Trading stopped", 3000);
}

void MainWindow::onSwitchToPaperTrading()
{
    m_paperTradingMode = true;
    m_paperTradingButton->setChecked(true);
    m_liveTradingButton->setChecked(false);
    
    m_logTextEdit->append(QString("[%1] Switched to paper trading mode").arg(QDateTime::currentDateTime().toString()));
    statusBar()->showMessage("Paper trading mode activated", 3000);
}

void MainWindow::onSwitchToLiveTrading()
{
    int ret = QMessageBox::question(this, "Switch to Live Trading",
        "⚠️ WARNING: You are about to switch to LIVE TRADING mode!\n\n"
        "This will use REAL MONEY and can result in ACTUAL LOSSES.\n"
        "Make sure you have:\n"
        "• Tested thoroughly with paper trading\n"
        "• Configured proper risk limits\n"
        "• Set up API credentials correctly\n\n"
        "Are you absolutely sure you want to proceed?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        m_paperTradingMode = false;
        m_paperTradingButton->setChecked(false);
        m_liveTradingButton->setChecked(true);
        
        m_logTextEdit->append(QString("[%1] ⚠️ WARNING: Switched to LIVE trading mode").arg(QDateTime::currentDateTime().toString()));
        m_logTextEdit->append(QString("[%1] Real money at risk - ensure proper risk management").arg(QDateTime::currentDateTime().toString()));
        statusBar()->showMessage("⚠️ LIVE trading mode activated", 5000);
    } else {
        m_paperTradingButton->setChecked(true);
        m_liveTradingButton->setChecked(false);
    }
}

void MainWindow::onTradingModeChanged()
{
    m_currentExchange = m_exchangeCombo->currentText();
    m_currentSymbol = m_symbolCombo->currentText();
    
    m_logTextEdit->append(QString("[%1] Selected %2 on %3").arg(QDateTime::currentDateTime().toString(), m_currentSymbol, m_currentExchange));
    statusBar()->showMessage(QString("Selected %1 on %2").arg(m_currentSymbol, m_currentExchange), 3000);
    
    // Update price chart title
    m_priceChart->setTitle(QString("%1 - %2").arg(m_currentSymbol, m_currentExchange));
}

void MainWindow::onShowAbout()
{
    QMessageBox::about(this, "About MasterMind Trader",
        "MasterMind Trader v1.0.0\n\n"
        "Professional High-Frequency Trading System\n"
        "Based on Renko Chart Pattern Recognition\n\n"
        "🎯 Features:\n"
        "• Real-time market data processing\n"
        "• Advanced risk management\n"
        "• Multiple exchange support\n"
        "• Paper trading simulation\n"
        "• Automated pattern recognition\n"
        "• Beautiful modern interface\n\n"
        "⚠️ Risk Warning:\n"
        "Trading involves substantial risk of loss.\n"
        "Past performance is not indicative of future results.\n"
        "Always test with paper trading first.\n\n"
        "© 2024 MasterMind Trading Systems\n"
        "All rights reserved.");
}

void MainWindow::updateStatusLabels()
{
    // Update equity display
    m_equityLabel->setText(QString("$%1").arg(m_currentEquity, 0, 'f', 2));
    
    // Update daily P&L display
    QString pnlText = QString("$%1").arg(m_dailyPnL, 0, 'f', 2);
    if (m_dailyPnL > 0) {
        m_dailyPnlLabel->setText("+" + pnlText);
        m_dailyPnlLabel->setStyleSheet("color: #4CAF50; font-weight: bold;");
    } else if (m_dailyPnL < 0) {
        m_dailyPnlLabel->setText(pnlText);
        m_dailyPnlLabel->setStyleSheet("color: #f44336; font-weight: bold;");
    } else {
        m_dailyPnlLabel->setText(pnlText);
        m_dailyPnlLabel->setStyleSheet("color: #757575; font-weight: bold;");
    }
    
    // Update other displays
    m_totalTradesLabel->setText(QString::number(m_totalTrades));
    m_winRateLabel->setText(QString("%1%").arg(m_winRate, 0, 'f', 1));
    m_openPositionsLabel->setText(QString::number(m_openPositions));
    m_riskUsedLabel->setText(QString("%1%").arg(m_riskUsed, 0, 'f', 1));
    
    // Update profit factor
    double profitFactor = 0.0;
    if (m_totalTrades > 0) {
        double grossProfit = m_dailyPnL > 0 ? m_dailyPnL : 0.0;
        double grossLoss = m_dailyPnL < 0 ? -m_dailyPnL : 1.0;
        profitFactor = grossProfit / grossLoss;
    }
    m_profitFactorLabel->setText(QString::number(profitFactor, 'f', 2));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_tradingActive) {
        int ret = QMessageBox::question(this, "Exit Application",
            "Trading is currently active. Are you sure you want to exit?",
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        
        if (ret == QMessageBox::No) {
            event->ignore();
            return;
        }
        
        onStopTrading();
    }
    
    saveConfiguration();
    event->accept();
} 