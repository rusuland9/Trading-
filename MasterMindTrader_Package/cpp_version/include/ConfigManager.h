#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    explicit ConfigManager(QObject *parent = nullptr);
    ~ConfigManager() = default;
    
    void loadConfig(const QString &filePath);
    void saveConfig(const QString &filePath);
    
    QJsonObject getConfig() const { return m_config; }
    void setConfig(const QJsonObject &config) { m_config = config; }
    
private:
    QJsonObject m_config;
};

#endif // CONFIGMANAGER_H 