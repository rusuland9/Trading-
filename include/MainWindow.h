#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QTabWidget>
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QTextEdit>
#include <QProgressBar>
#include <QTimer>
#include <QGroupBox>
#include <QFrame>
#include <QDockWidget>
#include <QAction>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSettings>
#include <QCloseEvent>
#include <memory>

// Forward declarations
class StrategyEngine;
class RiskManager;
class CapitalAllocator;
class OrderManager;
class ExchangeConnector;
class TradeSessionManager;
class Logger;
class ConfigManager;
class PaperTradeFallback;
class TradingDashboard;
class PositionWidget;
class ChartWidget;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onStartTrading();
    void onStopTrading();
    void onPauseTrading();
    void onShowSettings();
    void onShowAbout();
    void onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason);
    void onExitApplication();
    void onTradingModeChanged();
    void onStrategyStatusChanged();
    void onNewTrade();
    void onPositionUpdated();
    void onRiskLimitReached();
    void onConnectionStatusChanged();
    void onUpdateTimer();
    void onSwitchToPaperTrading();
    void onSwitchToLiveTrading();
    void onLoadConfiguration();
    void onSaveConfiguration();
    void onExportLogs();
    void onShowChart();
    void onShowPositions();
    void onShowLogs();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupCentralWidget();
    void setupDockWidgets();
    void setupSystemTray();
    void setupConnections();
    void setupTradingEngine();
    void setupStyles();
    void createControlPanel();
    void createStatusPanel();
    void createTradingPanel();
    void createRiskPanel();
    void updateStatusLabels();
    void updateTradingStatus();
    void updateRiskMetrics();
    void updatePositions();
    void updateLogs();
    void loadSettings();
    void saveSettings();
    void applyTheme();
    
    // UI Components
    QWidget *m_centralWidget;
    QSplitter *m_mainSplitter;
    QSplitter *m_leftSplitter;
    QSplitter *m_rightSplitter;
    QTabWidget *m_centralTabs;
    QTabWidget *m_rightTabs;
    
    // Panels
    QWidget *m_controlPanel;
    QWidget *m_statusPanel;
    QWidget *m_tradingPanel;
    QWidget *m_riskPanel;
    QWidget *m_chartPanel;
    QWidget *m_positionPanel;
    QWidget *m_logPanel;
    
    // Controls
    QPushButton *m_startButton;
    QPushButton *m_stopButton;
    QPushButton *m_pauseButton;
    QPushButton *m_settingsButton;
    QPushButton *m_paperTradingButton;
    QPushButton *m_liveTradingButton;
    
    QComboBox *m_exchangeCombo;
    QComboBox *m_symbolCombo;
    QComboBox *m_strategyCombo;
    QComboBox *m_timeframeCombo;
    
    QDoubleSpinBox *m_brickSizeSpinBox;
    QDoubleSpinBox *m_riskPercentSpinBox;
    QDoubleSpinBox *m_capitalSpinBox;
    QSpinBox *m_maxTradesSpinBox;
    
    QCheckBox *m_setup1CheckBox;
    QCheckBox *m_setup2CheckBox;
    QCheckBox *m_counterTradingCheckBox;
    QCheckBox *m_paperModeCheckBox;
    QCheckBox *m_swapAvoidanceCheckBox;
    
    // Status displays
    QLabel *m_connectionStatusLabel;
    QLabel *m_tradingStatusLabel;
    QLabel *m_strategyStatusLabel;
    QLabel *m_equityLabel;
    QLabel *m_pnlLabel;
    QLabel *m_dailyPnlLabel;
    QLabel *m_riskUsedLabel;
    QLabel *m_openPositionsLabel;
    QLabel *m_winRateLabel;
    QLabel *m_totalTradesLabel;
    
    // Tables and displays
    QTableWidget *m_positionsTable;
    QTableWidget *m_tradesTable;
    QTableWidget *m_ordersTable;
    QTextEdit *m_logTextEdit;
    TradingDashboard *m_dashboard;
    ChartWidget *m_chartWidget;
    PositionWidget *m_positionWidget;
    
    // Progress bars
    QProgressBar *m_riskProgressBar;
    QProgressBar *m_equityProgressBar;
    
    // Menu and toolbar
    QMenuBar *m_menuBar;
    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
    
    // System tray
    QSystemTrayIcon *m_systemTray;
    QMenu *m_trayMenu;
    
    // Actions
    QAction *m_exitAction;
    QAction *m_settingsAction;
    QAction *m_aboutAction;
    QAction *m_startTradingAction;
    QAction *m_stopTradingAction;
    QAction *m_pauseTradingAction;
    QAction *m_paperTradingAction;
    QAction *m_liveTradingAction;
    
    // Timers
    QTimer *m_updateTimer;
    QTimer *m_statusTimer;
    
    // Core trading components
    std::unique_ptr<StrategyEngine> m_strategyEngine;
    std::unique_ptr<RiskManager> m_riskManager;
    std::unique_ptr<CapitalAllocator> m_capitalAllocator;
    std::unique_ptr<OrderManager> m_orderManager;
    std::unique_ptr<ExchangeConnector> m_exchangeConnector;
    std::unique_ptr<TradeSessionManager> m_sessionManager;
    std::unique_ptr<Logger> m_logger;
    std::unique_ptr<ConfigManager> m_configManager;
    std::unique_ptr<PaperTradeFallback> m_paperTradeFallback;
    std::unique_ptr<SettingsDialog> m_settingsDialog;
    
    // Application state
    bool m_tradingActive;
    bool m_paperTradingMode;
    bool m_paused;
    QString m_currentSymbol;
    QString m_currentExchange;
    double m_currentEquity;
    double m_dailyPnl;
    int m_openPositions;
    int m_totalTrades;
    double m_winRate;
    double m_riskUsed;
    
    // Settings
    QSettings *m_settings;
};

#endif // MAINWINDOW_H 