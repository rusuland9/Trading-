#ifndef CAPITALALLOCATOR_H
#define CAPITALALLOCATOR_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <map>

class CapitalAllocator : public QObject
{
    Q_OBJECT

public:
    explicit CapitalAllocator(QObject *parent = nullptr);
    ~CapitalAllocator() = default;
    
    void setCapitalForSymbol(const QString &symbol, double capital);
    double getCapitalForSymbol(const QString &symbol) const;
    double getTotalCapital() const;
    
private:
    std::map<QString, double> m_capitalAllocation;
    mutable QMutex m_mutex;
};

#endif // CAPITALALLOCATOR_H 