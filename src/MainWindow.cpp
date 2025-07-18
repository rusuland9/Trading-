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
{
    setWindowTitle("MasterMind Trader - Professional Trading System");
    setMinimumSize(1200, 800);
    resize(1600, 1000);
    
    setupUI();
    setupMenuBar();
    setupStatusBar();
    setupConnections();
    
    // Start update timer
    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(1000);
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::updateStatusLabels);
    m_updateTimer->start();
    
    // Apply dark theme
    applyTheme();
    
    // Initial status update
    updateStatusLabels();
}

MainWindow::~MainWindow()
{
    if (m_tradingActive) {
        onStopTrading();
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
    m_paperTradingButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 6px; border-radius: 4px; } QPushButton:checked { background-color: #0D47A1; }");
    
    m_liveTradingButton = new QPushButton("Live Trading");
    m_liveTradingButton->setCheckable(true);
    m_liveTradingButton->setStyleSheet("QPushButton { background-color: #757575; color: white; padding: 6px; border-radius: 4px; } QPushButton:checked { background-color: #FF5722; }");
    
    modeLayout->addWidget(m_paperTradingButton);
    modeLayout->addWidget(m_liveTradingButton);
    controlLayout->addLayout(modeLayout);
    
    // Exchange selection
    QFormLayout *selectionLayout = new QFormLayout;
    
    m_exchangeCombo = new QComboBox;
    m_exchangeCombo->addItems({"Binance", "Coinbase", "Deribit", "Delta Exchange", "MetaTrader 4", "MetaTrader 5"});
    selectionLayout->addRow("Exchange:", m_exchangeCombo);
    
    m_symbolCombo = new QComboBox;
    m_symbolCombo->addItems({"BTCUSD", "ETHUSD", "EURUSD", "GBPUSD", "XAUUSD", "USDJPY"});
    selectionLayout->addRow("Symbol:", m_symbolCombo);
    
    controlLayout->addLayout(selectionLayout);
    
    leftLayout->addWidget(controlGroup);
    
    // Status Panel
    QGroupBox *statusGroup = new QGroupBox("System Status");
    QGridLayout *statusLayout = new QGridLayout(statusGroup);
    
    statusLayout->addWidget(new QLabel("Connection:"), 0, 0);
    m_connectionStatusLabel = new QLabel("Disconnected");
    m_connectionStatusLabel->setStyleSheet("color: #f44336;");
    statusLayout->addWidget(m_connectionStatusLabel, 0, 1);
    
    statusLayout->addWidget(new QLabel("Trading:"), 1, 0);
    m_tradingStatusLabel = new QLabel("Stopped");
    m_tradingStatusLabel->setStyleSheet("color: #f44336;");
    statusLayout->addWidget(m_tradingStatusLabel, 1, 1);
    
    statusLayout->addWidget(new QLabel("Equity:"), 2, 0);
    m_equityLabel = new QLabel("$10,000.00");
    m_equityLabel->setStyleSheet("color: #4CAF50; font-weight: bold;");
    statusLayout->addWidget(m_equityLabel, 2, 1);
    
    statusLayout->addWidget(new QLabel("Daily P&L:"), 3, 0);
    m_dailyPnlLabel = new QLabel("$0.00");
    m_dailyPnlLabel->setStyleSheet("color: #757575; font-weight: bold;");
    statusLayout->addWidget(m_dailyPnlLabel, 3, 1);
    
    leftLayout->addWidget(statusGroup);
    
    // Risk Panel
    QGroupBox *riskGroup = new QGroupBox("Risk Management");
    QFormLayout *riskLayout = new QFormLayout(riskGroup);
    
    m_riskPercentSpinBox = new QDoubleSpinBox;
    m_riskPercentSpinBox->setRange(0.1, 10.0);
    m_riskPercentSpinBox->setValue(2.0);
    m_riskPercentSpinBox->setSuffix("%");
    riskLayout->addRow("Risk per Trade:", m_riskPercentSpinBox);
    
    m_capitalSpinBox = new QDoubleSpinBox;
    m_capitalSpinBox->setRange(100.0, 1000000.0);
    m_capitalSpinBox->setValue(10000.0);
    m_capitalSpinBox->setPrefix("$");
    riskLayout->addRow("Capital:", m_capitalSpinBox);
    
    m_riskProgressBar = new QProgressBar;
    m_riskProgressBar->setRange(0, 100);
    m_riskProgressBar->setValue(0);
    riskLayout->addRow("Risk Usage:", m_riskProgressBar);
    
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
    
    // Open Positions
    QLabel *positionsLabel = new QLabel("Open Positions:");
    positionsLabel->setStyleSheet("font-weight: bold;");
    m_openPositionsLabel = new QLabel("0");
    m_openPositionsLabel->setStyleSheet("font-size: 24px; color: #FF9800;");
    metricsLayout->addWidget(positionsLabel, 2, 0);
    metricsLayout->addWidget(m_openPositionsLabel, 2, 1);
    
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
    m_positionsTable->setColumnCount(6);
    m_positionsTable->setHorizontalHeaderLabels({"Symbol", "Side", "Size", "Entry Price", "Current Price", "P&L"});
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
    
    // Add some initial log entries
    m_logTextEdit->append(QString("[%1] MasterMind Trader initialized").arg(QDateTime::currentDateTime().toString()));
    m_logTextEdit->append(QString("[%1] System ready for trading").arg(QDateTime::currentDateTime().toString()));
}

void MainWindow::setupMenuBar()
{
    // File menu
    QMenu *fileMenu = menuBar()->addMenu("&File");
    
    QAction *exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAction);
    
    // Trading menu
    QMenu *tradingMenu = menuBar()->addMenu("&Trading");
    
    QAction *startAction = new QAction("&Start Trading", this);
    connect(startAction, &QAction::triggered, this, &MainWindow::onStartTrading);
    tradingMenu->addAction(startAction);
    
    QAction *stopAction = new QAction("St&op Trading", this);
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
    statusBar()->addPermanentWidget(new QLabel("Version 1.0.0"));
}

void MainWindow::setupConnections()
{
    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::onStartTrading);
    connect(m_stopButton, &QPushButton::clicked, this, &MainWindow::onStopTrading);
    connect(m_paperTradingButton, &QPushButton::clicked, this, &MainWindow::onSwitchToPaperTrading);
    connect(m_liveTradingButton, &QPushButton::clicked, this, &MainWindow::onSwitchToLiveTrading);
    connect(m_exchangeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onTradingModeChanged);
    connect(m_symbolCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onTradingModeChanged);
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
        
        QDoubleSpinBox {
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
    
    // Simulate connection
    m_connectionStatusLabel->setText("Connected");
    m_connectionStatusLabel->setStyleSheet("color: #4CAF50;");
    
    m_logTextEdit->append(QString("[%1] Trading started").arg(QDateTime::currentDateTime().toString()));
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
    
    m_connectionStatusLabel->setText("Disconnected");
    m_connectionStatusLabel->setStyleSheet("color: #f44336;");
    
    m_logTextEdit->append(QString("[%1] Trading stopped").arg(QDateTime::currentDateTime().toString()));
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
        "Are you sure you want to switch to live trading mode?\n"
        "This will use real money and can result in losses.",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        m_paperTradingMode = false;
        m_paperTradingButton->setChecked(false);
        m_liveTradingButton->setChecked(true);
        
        m_logTextEdit->append(QString("[%1] WARNING: Switched to live trading mode").arg(QDateTime::currentDateTime().toString()));
        statusBar()->showMessage("Live trading mode activated", 3000);
    } else {
        m_paperTradingButton->setChecked(true);
        m_liveTradingButton->setChecked(false);
    }
}

void MainWindow::onTradingModeChanged()
{
    QString exchange = m_exchangeCombo->currentText();
    QString symbol = m_symbolCombo->currentText();
    
    m_logTextEdit->append(QString("[%1] Selected %2 on %3").arg(QDateTime::currentDateTime().toString(), symbol, exchange));
    statusBar()->showMessage(QString("Selected %1 on %2").arg(symbol, exchange), 3000);
}

void MainWindow::onShowAbout()
{
    QMessageBox::about(this, "About MasterMind Trader",
        "MasterMind Trader v1.0.0\n\n"
        "Professional High-Frequency Trading System\n"
        "Based on Renko Chart Pattern Recognition\n\n"
        "Features:\n"
        "• Real-time market data processing\n"
        "• Advanced risk management\n"
        "• Multiple exchange support\n"
        "• Paper trading simulation\n"
        "• Automated pattern recognition\n\n"
        "Copyright © 2024 MasterMind Trading Systems\n"
        "All rights reserved.");
}

void MainWindow::updateStatusLabels()
{
    // Update equity display
    m_equityLabel->setText(QString("$%1").arg(m_currentEquity, 0, 'f', 2));
    
    // Simulate some trading activity if active
    if (m_tradingActive) {
        static int counter = 0;
        counter++;
        
        if (counter % 10 == 0) {
            // Simulate small equity changes
            double change = (qrand() % 200 - 100) / 100.0;
            m_currentEquity += change;
            m_dailyPnL += change;
            
            // Update risk usage
            m_riskUsed = (qrand() % 30) + 10; // Random between 10-40%
            m_riskProgressBar->setValue(static_cast<int>(m_riskUsed));
            
            // Simulate some trades
            if (counter % 50 == 0) {
                m_totalTrades++;
                m_winRate = (qrand() % 40) + 60; // Random between 60-100%
                
                m_logTextEdit->append(QString("[%1] Simulated trade #%2 completed")
                    .arg(QDateTime::currentDateTime().toString())
                    .arg(m_totalTrades));
            }
        }
    }
    
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
    
    event->accept();
} 