#ifndef WEBSOCKETS_H
#define WEBSOCKETS_H

#include "mainwindow.h"
#include "precompiled.h"


QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WSServer : public QObject
{
    Q_OBJECT
public:
    explicit WSServer(quint16 port, bool debug = false, QObject *parent = nullptr);
    ~WSServer();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    MainWindow *m_mainWindow;
    QList<QWebSocket *> m_clients;
    bool m_debug;
};

enum NonceType {
    LOCAL = 1,
    REMOTE
};

class AppDataServer {
public:
    static void          connectAppDialog(QWidget* parent);

    static void          processSendTx(QJsonObject sendTx, MainWindow* mainwindow, QWebSocket* pClient);
    static void          processMessage(QString message, MainWindow* mainWindow, QWebSocket* pClient);
    static void          processDecryptedMessage(QString message, MainWindow* mainWindow, QWebSocket* pClient);
    static void          processGetInfo(MainWindow* mainWindow, QWebSocket* pClient);
    static void          processGetTransactions(MainWindow* mainWindow, QWebSocket* pClient);

    static QString       decryptMessage(QJsonDocument msg, QString secretHex);
    static QString       encryptOutgoing(QString msg);

    static QString       getSecretHex();
    static void          saveNewSecret(QString secretHex);

    static QString       getNonceHex(NonceType nt);
    static void          saveNonceHex(NonceType nt, QString noncehex);
};

class AppDataModel {
public:
    static AppDataModel* getInstance() {
        if (instance == NULL)
            instance = new AppDataModel();

        return instance;
    }

    double getTBalance()     { return balTransparent;  }
    double getZBalance()     { return balShielded; }
    double getTotalBalance() { return balTotal; }

    void   setBalances(double transparent, double shielded) {
        balTransparent = transparent;
        balShielded = shielded;
        balTotal = balTransparent + balShielded;
    }

private:
    AppDataModel() = default;   // Private, for singleton

    double balTransparent;
    double balShielded;
    double balTotal;
    double balMaxSingle;

    QString saplingAddress;

    static AppDataModel* instance;
};

#endif // WEBSOCKETS_H