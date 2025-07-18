#include "ChartWidget.h"

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupChart();
}

void ChartWidget::setupUI()
{
    m_layout = new QVBoxLayout(this);
    
    m_groupBox = new QGroupBox("Price Chart");
    m_layout->addWidget(m_groupBox);
    
    QVBoxLayout *groupLayout = new QVBoxLayout(m_groupBox);
    
    // Control panel
    QHBoxLayout *controlLayout = new QHBoxLayout;
    
    m_symbolCombo = new QComboBox;
    m_symbolCombo->addItems({"BTCUSD", "ETHUSD", "EURUSD", "GBPUSD", "USDJPY", "XAUUSD"});
    controlLayout->addWidget(m_symbolCombo);
    
    m_refreshButton = new QPushButton("Refresh");
    m_refreshButton->setStyleSheet("QPushButton { background-color: #42A5F5; color: white; font-weight: bold; padding: 6px 12px; border-radius: 4px; }");
    controlLayout->addWidget(m_refreshButton);
    
    controlLayout->addStretch();
    groupLayout->addLayout(controlLayout);
    
    // Chart view
    m_chartView = new QChartView;
    m_chartView->setRenderHint(QPainter::Antialiasing);
    groupLayout->addWidget(m_chartView);
}

void ChartWidget::setupChart()
{
    m_chart = new QChart;
    m_chart->setTitle("Price Chart");
    m_chart->setAnimationOptions(QChart::NoAnimation);
    
    // Create series
    m_priceSeries = new QLineSeries;
    m_priceSeries->setName("Price");
    m_chart->addSeries(m_priceSeries);
    
    // Create axes
    m_timeAxis = new QDateTimeAxis;
    m_timeAxis->setFormat("hh:mm:ss");
    m_timeAxis->setTitleText("Time");
    m_chart->addAxis(m_timeAxis, Qt::AlignBottom);
    m_priceSeries->attachAxis(m_timeAxis);
    
    m_priceAxis = new QValueAxis;
    m_priceAxis->setTitleText("Price");
    m_chart->addAxis(m_priceAxis, Qt::AlignLeft);
    m_priceSeries->attachAxis(m_priceAxis);
    
    // Set chart to chart view
    m_chartView->setChart(m_chart);
}

void ChartWidget::updateChart(const QString &symbol)
{
    Q_UNUSED(symbol)
    // Stub implementation
}

void ChartWidget::addDataPoint(double price, const QDateTime &time)
{
    m_priceSeries->append(time.toMSecsSinceEpoch(), price);
    
    // Keep only last 100 points
    if (m_priceSeries->count() > 100) {
        m_priceSeries->removePoints(0, m_priceSeries->count() - 100);
    }
    
    // Update axes ranges
    m_timeAxis->setRange(time.addSecs(-3600), time); // Last hour
    
    // Auto-scale price axis
    auto points = m_priceSeries->points();
    if (!points.isEmpty()) {
        double minPrice = points.first().y();
        double maxPrice = points.first().y();
        
        for (const auto &point : points) {
            minPrice = qMin(minPrice, point.y());
            maxPrice = qMax(maxPrice, point.y());
        }
        
        double range = maxPrice - minPrice;
        m_priceAxis->setRange(minPrice - range * 0.1, maxPrice + range * 0.1);
    }
}

void ChartWidget::clearChart()
{
    m_priceSeries->clear();
} 