#ifndef TRADINGDASHBOARD_H
#define TRADINGDASHBOARD_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QFrame>
#include <QTimer>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QAreaSeries>
#include <QLegend>
#include <QGroupBox>
#include <QTableWidget>
#include <QHeaderView>
#include <memory>

QT_CHARTS_USE_NAMESPACE

struct DashboardMetrics {
    double totalEquity;
    double dailyPnL;
    double dailyReturn;
    double maxDrawdown;
    double winRate;
    int totalTrades;
    int winningTrades;
    int losingTrades;
    double profitFactor;
    double sharpeRatio;
    double averageWin;
    double averageLoss;
    double largestWin;
    double largestLoss;
    QDateTime lastUpdate;
};

class TradingDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit TradingDashboard(QWidget *parent = nullptr);
    ~TradingDashboard();
    
    void updateMetrics(const DashboardMetrics &metrics);
    void updateEquityCurve(const std::vector<std::pair<QDateTime, double>> &data);
    void updatePnLChart(const std::vector<std::pair<QDateTime, double>> &data);
    void updateDrawdownChart(const std::vector<std::pair<QDateTime, double>> &data);
    void updateRecentTrades(const std::vector<Position> &trades);
    
    void setRealTimeMode(bool enabled);
    void resetDashboard();
    
    DashboardMetrics getMetrics() const { return m_metrics; }

signals:
    void metricsUpdated(const DashboardMetrics &metrics);
    void chartClicked(const QString &chartType);
    void tradeSelected(const QString &tradeId);

private slots:
    void onUpdateTimer();
    void onChartHovered(const QPointF &point, bool state);
    void onEquityChartClicked();
    void onPnLChartClicked();
    void onDrawdownChartClicked();
    void onTradeTableClicked(int row, int column);
    void onRefreshClicked();
    void onExportClicked();

private:
    void setupUI();
    void setupEquityChart();
    void setupPnLChart();
    void setupDrawdownChart();
    void setupMetricsDisplay();
    void setupRecentTradesTable();
    void setupControlButtons();
    void applyStyles();
    
    void updateEquityMetrics();
    void updatePnLMetrics();
    void updateDrawdownMetrics();
    void updateTradeStatistics();
    void updateProgressBars();
    
    void formatMetricsLabels();
    void formatCharts();
    void formatTables();
    
    // UI Components
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_topLayout;
    QHBoxLayout *m_middleLayout;
    QHBoxLayout *m_bottomLayout;
    
    // Metrics display
    QGroupBox *m_metricsGroup;
    QGridLayout *m_metricsLayout;
    
    QLabel *m_equityLabel;
    QLabel *m_equityValueLabel;
    QLabel *m_dailyPnLLabel;
    QLabel *m_dailyPnLValueLabel;
    QLabel *m_dailyReturnLabel;
    QLabel *m_dailyReturnValueLabel;
    QLabel *m_maxDrawdownLabel;
    QLabel *m_maxDrawdownValueLabel;
    QLabel *m_winRateLabel;
    QLabel *m_winRateValueLabel;
    QLabel *m_totalTradesLabel;
    QLabel *m_totalTradesValueLabel;
    QLabel *m_profitFactorLabel;
    QLabel *m_profitFactorValueLabel;
    QLabel *m_sharpeRatioLabel;
    QLabel *m_sharpeRatioValueLabel;
    
    // Progress bars
    QProgressBar *m_winRateProgress;
    QProgressBar *m_drawdownProgress;
    QProgressBar *m_profitFactorProgress;
    
    // Charts
    QGroupBox *m_chartsGroup;
    QChart *m_equityChart;
    QChartView *m_equityChartView;
    QLineSeries *m_equitySeries;
    QAreaSeries *m_equityAreaSeries;
    QValueAxis *m_equityValueAxis;
    QDateTimeAxis *m_equityDateAxis;
    
    QChart *m_pnlChart;
    QChartView *m_pnlChartView;
    QLineSeries *m_pnlSeries;
    QValueAxis *m_pnlValueAxis;
    QDateTimeAxis *m_pnlDateAxis;
    
    QChart *m_drawdownChart;
    QChartView *m_drawdownChartView;
    QLineSeries *m_drawdownSeries;
    QAreaSeries *m_drawdownAreaSeries;
    QValueAxis *m_drawdownValueAxis;
    QDateTimeAxis *m_drawdownDateAxis;
    
    // Recent trades table
    QGroupBox *m_tradesGroup;
    QTableWidget *m_tradesTable;
    
    // Control buttons
    QFrame *m_controlFrame;
    QPushButton *m_refreshButton;
    QPushButton *m_exportButton;
    QPushButton *m_settingsButton;
    QPushButton *m_fullScreenButton;
    
    // Data
    DashboardMetrics m_metrics;
    std::vector<std::pair<QDateTime, double>> m_equityData;
    std::vector<std::pair<QDateTime, double>> m_pnlData;
    std::vector<std::pair<QDateTime, double>> m_drawdownData;
    std::vector<Position> m_recentTrades;
    
    // Settings
    bool m_realTimeMode;
    bool m_autoRefresh;
    int m_updateInterval;
    int m_maxDataPoints;
    
    // Timer
    QTimer *m_updateTimer;
    
    // Constants
    static const int DEFAULT_UPDATE_INTERVAL = 1000; // 1 second
    static const int MAX_DATA_POINTS = 1000;
    static const int MAX_RECENT_TRADES = 50;
    
    // Chart colors
    static const QColor EQUITY_COLOR;
    static const QColor PNL_POSITIVE_COLOR;
    static const QColor PNL_NEGATIVE_COLOR;
    static const QColor DRAWDOWN_COLOR;
    static const QColor BACKGROUND_COLOR;
    static const QColor GRID_COLOR;
    static const QColor TEXT_COLOR;
};

#endif // TRADINGDASHBOARD_H 