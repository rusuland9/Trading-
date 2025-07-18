#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>

QT_CHARTS_USE_NAMESPACE

class ChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget() = default;
    
    void updateChart(const QString &symbol);
    void addDataPoint(double price, const QDateTime &time);
    void clearChart();
    
private:
    void setupUI();
    void setupChart();
    
    QVBoxLayout *m_layout;
    QGroupBox *m_groupBox;
    QChart *m_chart;
    QChartView *m_chartView;
    QLineSeries *m_priceSeries;
    QValueAxis *m_priceAxis;
    QDateTimeAxis *m_timeAxis;
    QComboBox *m_symbolCombo;
    QPushButton *m_refreshButton;
};

#endif // CHARTWIDGET_H 