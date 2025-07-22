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
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QTableWidgetItem>
#include <memory>

#ifndef QT_CHARTS_USE_NAMESPACE
#define QT_CHARTS_USE_NAMESPACE
#endif

QT_CHARTS_USE_NAMESPACE

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
    void onShowAbout();
    void onTradingModeChanged();
    void onSwitchToPaperTrading();
    void onSwitchToLiveTrading();
    void updateStatusLabels();
    void simulateTrading();
    void simulateTradeSignal();

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupConnections();
    void setupPriceChart();
    void applyTheme();
    void loadConfiguration();
    void saveConfiguration();
    void addTradeToTable(const QString &side, double size, double entryPrice, double pnl, const QString &status);
    
    // UI Components
    QWidget *m_centralWidget;
    QTabWidget *m_centralTabs;
    
    // Controls
    QPushButton *m_startButton;
    QPushButton *m_stopButton;
    QPushButton *m_paperTradingButton;
    QPushButton *m_liveTradingButton;
    
    QComboBox *m_exchangeCombo;
    QComboBox *m_symbolCombo;
    
    QDoubleSpinBox *m_brickSizeSpinBox;
    QDoubleSpinBox *m_riskPercentSpinBox;
    QDoubleSpinBox *m_capitalSpinBox;
    QSpinBox *m_maxTradesSpinBox;
    
    QCheckBox *m_setup1CheckBox;
    QCheckBox *m_setup2CheckBox;
    QCheckBox *m_counterTradingCheckBox;
    
    // Status displays
    QLabel *m_connectionStatusLabel;
    QLabel *m_tradingStatusLabel;
    QLabel *m_priceLabel;
    QLabel *m_equityLabel;
    QLabel *m_dailyPnlLabel;
    QLabel *m_openPositionsLabel;
    QLabel *m_winRateLabel;
    QLabel *m_totalTradesLabel;
    QLabel *m_profitFactorLabel;
    QLabel *m_riskUsedLabel;
    QLabel *m_connectionIndicator;
    QLabel *m_timeLabel;
    
    // Tables and displays
    QTableWidget *m_positionsTable;
    QTextEdit *m_logTextEdit;
    
    // Progress bars
    QProgressBar *m_riskProgressBar;
    
    // Chart components
    QWidget *m_chartWidget;
    QChart *m_priceChart;
    QChartView *m_chartView;
    QLineSeries *m_priceSeries;
    QValueAxis *m_priceAxis;
    QDateTimeAxis *m_timeAxis;
    
    // Timers
    QTimer *m_updateTimer;
    QTimer *m_simulationTimer;
    
    // Application state
    bool m_tradingActive;
    bool m_paperTradingMode;
    bool m_paused;
    QString m_currentSymbol;
    QString m_currentExchange;
    
    // Trading data
    double m_currentEquity;
    double m_dailyPnL;
    int m_openPositions;
    int m_totalTrades;
    int m_winningTrades;
    double m_winRate;
    double m_riskUsed;
    
    // Risk management
    double m_maxRiskPerTrade;
    double m_maxDailyRisk;
    int m_maxOpenPositions;
    
    // Simulation variables
    double m_currentPrice;
    int m_priceDirection;
    int m_simulationCounter;
    double m_spread;
    double m_slippage;
    double m_priceVolatility;
};

#endif // MAINWINDOW_H 