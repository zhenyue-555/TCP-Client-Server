#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTextBrowser>
#include <QPushButton>
#include <QLineEdit>

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void onConnectButtonClicked();
    void onDisconnectButtonClicked();
    void onSendButtonClicked();
    void readFromServer();

private:
    QLineEdit *serverAddressEdit;
    QLineEdit *clientIdEdit;
    QLineEdit *textToSendEdit;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QPushButton *sendButton;
    QTextBrowser *systemLogBrowser;
    QTextBrowser *messageReceivedBrowser;
    QTcpSocket *clientSocket;
};

#endif
