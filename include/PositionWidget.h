#ifndef POSITIONWIDGET_H
#define POSITIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

class PositionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PositionWidget(QWidget *parent = nullptr);
    ~PositionWidget() = default;
    
    void updatePositions();
    void addPosition(const QString &symbol, const QString &side, double size, double price);
    void removePosition(const QString &symbol);
    
private:
    void setupUI();
    
    QVBoxLayout *m_layout;
    QGroupBox *m_groupBox;
    QTableWidget *m_positionsTable;
    QLabel *m_totalPnLLabel;
    QPushButton *m_closeAllButton;
};

#endif // POSITIONWIDGET_H 