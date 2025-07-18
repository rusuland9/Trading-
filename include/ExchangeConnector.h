#ifndef EXCHANGECONNECTOR_H
#define EXCHANGECONNECTOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QMutex>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSslConfiguration>
#include <QWebSocket>
#include <memory>
#include <map>

enum class ExchangeType {
    BINANCE,
    COINBASE,
    DERIBIT,
    DELTA_EXCHANGE,
    METATRADER4,
    METATRADER5
};

enum class OrderType {
    MARKET,
    LIMIT,
    STOP,
    STOP_LIMIT,
    TRAILING_STOP,
    ICEBERG
};

enum class OrderSide {
    BUY,
    SELL
};

enum class OrderStatus {
    PENDING,
    FILLED,
    PARTIALLY_FILLED,
    CANCELLED,
    REJECTED,
    EXPIRED
};

struct MarketData {
    QString symbol;
    double bid;
    double ask;
    double last;
    double volume;
    double high24h;
    double low24h;
    double change24h;
    QDateTime timestamp;
};

struct OrderRequest {
    QString symbol;
    OrderType type;
    OrderSide side;
    double quantity;
    double price;
    double stopPrice;
    double timeInForce;
    QString clientOrderId;
    QJsonObject metadata;
};

struct OrderResponse {
    QString orderId;
    QString clientOrderId;
    OrderStatus status;
    double filledQuantity;
    double averagePrice;
    double commission;
    QString error;
    QDateTime timestamp;
};

struct AccountInfo {
    double totalBalance;
    double availableBalance;
    double usedMargin;
    double freeMargin;
    double marginLevel;
    double equity;
    QString currency;
    QDateTime lastUpdate;
};

class ExchangeConnector : public QObject
{
    Q_OBJECT

public:
    explicit ExchangeConnector(QObject *parent = nullptr);
    ~ExchangeConnector();
    
    // Connection management
    void setExchange(ExchangeType exchange);
    void setApiCredentials(const QString &apiKey, const QString &apiSecret, const QString &passphrase = "");
    void setTestMode(bool enabled);
    
    bool connect();
    void disconnect();
    bool isConnected() const;
    
    // Market data
    void subscribeToMarketData(const QString &symbol);
    void unsubscribeFromMarketData(const QString &symbol);
    MarketData getMarketData(const QString &symbol) const;
    
    // Trading operations
    QString placeOrder(const OrderRequest &request);
    bool cancelOrder(const QString &orderId);
    bool modifyOrder(const QString &orderId, double newPrice, double newQuantity = 0);
    OrderResponse getOrderStatus(const QString &orderId);
    std::vector<OrderResponse> getOpenOrders(const QString &symbol = "");
    std::vector<OrderResponse> getOrderHistory(const QString &symbol = "", int limit = 100);
    
    // Account information
    AccountInfo getAccountInfo();
    std::vector<Position> getPositions(const QString &symbol = "");
    std::vector<Position> getTradeHistory(const QString &symbol = "", int limit = 100);
    
    // Exchange specific features
    std::vector<QString> getAvailableSymbols();
    std::map<QString, double> getSymbolInfo(const QString &symbol);
    double getMinOrderSize(const QString &symbol);
    double getMaxOrderSize(const QString &symbol);
    double getTickSize(const QString &symbol);
    double getCommissionRate(const QString &symbol);
    double getSwapRate(const QString &symbol, OrderSide side);
    
    // Connection status
    ExchangeType getCurrentExchange() const { return m_currentExchange; }
    QString getExchangeName() const;
    QString getConnectionStatus() const;
    QDateTime getLastHeartbeat() const { return m_lastHeartbeat; }
    
    // Error handling
    QString getLastError() const { return m_lastError; }
    bool hasError() const { return !m_lastError.isEmpty(); }
    void clearError() { m_lastError.clear(); }

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    void marketDataReceived(const MarketData &data);
    void orderFilled(const OrderResponse &response);
    void orderCancelled(const QString &orderId);
    void orderRejected(const QString &orderId, const QString &reason);
    void positionUpdated(const Position &position);
    void accountUpdated(const AccountInfo &info);
    void errorOccurred(const QString &error);

private slots:
    void onNetworkReplyFinished();
    void onWebSocketConnected();
    void onWebSocketDisconnected();
    void onWebSocketTextMessageReceived(const QString &message);
    void onWebSocketError(QAbstractSocket::SocketError error);
    void onHeartbeatTimer();
    void onReconnectTimer();

private:
    // Exchange-specific implementations
    void connectBinance();
    void connectCoinbase();
    void connectDeribit();
    void connectDeltaExchange();
    void connectMetaTrader4();
    void connectMetaTrader5();
    
    // API request methods
    QNetworkRequest createRequest(const QString &endpoint, const QJsonObject &params = QJsonObject());
    QJsonObject sendRequest(const QString &method, const QString &endpoint, const QJsonObject &params = QJsonObject());
    QString signRequest(const QString &queryString, const QString &secret);
    
    // WebSocket methods
    void setupWebSocket();
    void sendWebSocketMessage(const QJsonObject &message);
    void handleWebSocketMessage(const QJsonObject &message);
    
    // Order management
    QString generateClientOrderId();
    void processOrderResponse(const QJsonObject &response);
    void updateOrderStatus(const QString &orderId, OrderStatus status);
    
    // Market data processing
    void processMarketData(const QJsonObject &data);
    void updateMarketData(const QString &symbol, const MarketData &data);
    
    // Helper methods
    QString formatSymbol(const QString &symbol) const;
    double formatPrice(double price, const QString &symbol) const;
    double formatQuantity(double quantity, const QString &symbol) const;
    OrderStatus parseOrderStatus(const QString &status) const;
    
    // Binance specific methods
    QString binancePlaceOrder(const OrderRequest &request);
    QJsonObject binanceGetAccountInfo();
    void binanceSubscribeMarketData(const QString &symbol);
    
    // Coinbase specific methods
    QString coinbasePlaceOrder(const OrderRequest &request);
    QJsonObject coinbaseGetAccountInfo();
    void coinbaseSubscribeMarketData(const QString &symbol);
    
    // Deribit specific methods
    QString deribitPlaceOrder(const OrderRequest &request);
    QJsonObject deribitGetAccountInfo();
    void deribitSubscribeMarketData(const QString &symbol);
    
    // Delta Exchange specific methods
    QString deltaPlaceOrder(const OrderRequest &request);
    QJsonObject deltaGetAccountInfo();
    void deltaSubscribeMarketData(const QString &symbol);
    
    // MetaTrader specific methods
    QString metatraderPlaceOrder(const OrderRequest &request);
    QJsonObject metatraderGetAccountInfo();
    void metatraderSubscribeMarketData(const QString &symbol);
    
    // Member variables
    ExchangeType m_currentExchange;
    QString m_apiKey;
    QString m_apiSecret;
    QString m_passphrase;
    bool m_testMode;
    bool m_connected;
    
    // Network components
    QNetworkAccessManager *m_networkManager;
    QWebSocket *m_webSocket;
    QTimer *m_heartbeatTimer;
    QTimer *m_reconnectTimer;
    
    // Data storage
    std::map<QString, MarketData> m_marketData;
    std::map<QString, OrderResponse> m_orders;
    std::map<QString, Position> m_positions;
    AccountInfo m_accountInfo;
    
    // Connection state
    QString m_lastError;
    QDateTime m_lastHeartbeat;
    QDateTime m_lastReconnect;
    int m_reconnectAttempts;
    
    // API endpoints (exchange-specific)
    std::map<QString, QString> m_apiEndpoints;
    
    // Thread safety
    mutable QMutex m_mutex;
    
    // Constants
    static const int HEARTBEAT_INTERVAL = 30000; // 30 seconds
    static const int RECONNECT_INTERVAL = 5000; // 5 seconds
    static const int MAX_RECONNECT_ATTEMPTS = 10;
    static const int REQUEST_TIMEOUT = 30000; // 30 seconds
    
    // Rate limiting
    QTimer *m_rateLimitTimer;
    int m_requestCount;
    QDateTime m_lastRequestTime;
    static const int MAX_REQUESTS_PER_SECOND = 10;
};

#endif // EXCHANGECONNECTOR_H 