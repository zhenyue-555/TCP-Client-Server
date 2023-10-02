#include "client.h"
#include <QGridLayout>
#include <QLabel>
#include <QDateTime>
#include <QHostAddress>

Client::Client(QWidget *parent) : QWidget(parent) {
    serverAddressEdit = new QLineEdit(this);
    serverAddressEdit->setText("127.0.0.1");
    clientIdEdit = new QLineEdit(this);
    connectButton = new QPushButton("Connect", this);
    disconnectButton = new QPushButton("Disconnect", this);
    sendButton = new QPushButton("Send", this);
    systemLogBrowser = new QTextBrowser(this);
    messageReceivedBrowser = new QTextBrowser(this);
    textToSendEdit = new QLineEdit(this);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(new QLabel("Server Address:"), 0, 0);
    mainLayout->addWidget(serverAddressEdit, 0, 1);
    mainLayout->addWidget(new QLabel("Client ID:"), 0, 2);
    mainLayout->addWidget(clientIdEdit, 0, 3);
    mainLayout->addWidget(connectButton, 0, 4);
    mainLayout->addWidget(disconnectButton, 0, 5);
    mainLayout->addWidget(new QLabel("System Log:"), 1, 0);
    mainLayout->addWidget(systemLogBrowser, 2, 0, 1, 3);
    mainLayout->addWidget(new QLabel("Message Received:"), 1, 3);
    mainLayout->addWidget(messageReceivedBrowser, 2, 3, 1, 3);
    mainLayout->addWidget(sendButton, 3, 4);
    mainLayout->addWidget(textToSendEdit, 3, 0, 1, 4);
    setLayout(mainLayout);

    clientSocket = new QTcpSocket(this);
    connect(connectButton, &QPushButton::clicked, this, &Client::onConnectButtonClicked);
    connect(disconnectButton, &QPushButton::clicked, this, &Client::onDisconnectButtonClicked);
    connect(sendButton, &QPushButton::clicked, this, &Client::onSendButtonClicked);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Client::readFromServer);
}

void Client::onConnectButtonClicked() {
    QString clientIdStr = clientIdEdit->text().trimmed();  // Trim to remove any leading or trailing spaces

    bool isInt;
    int clientId = clientIdStr.toInt(&isInt);  // Convert to integer and check if the conversion is successful

    if (!isInt) {
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        systemLogBrowser->append(currentTime + ": Error: Client ID must be an integer.");
        return;  // Do not proceed if the client ID is not an integer
    }
    clientSocket->connectToHost(serverAddressEdit->text(), 1502);

    if (clientSocket->waitForConnected(3000)) {  // wait for 3 seconds
        QString connectionMessage = QString("C:%1:").arg(clientIdEdit->text());
        clientSocket->write(connectionMessage.toUtf8());
    } else {
        systemLogBrowser->append("Failed to connect to server, check if address is correct or server is running.");
    }
}


void Client::onSendButtonClicked() {
    // Check if connected to the server
    if (clientSocket->state() != QAbstractSocket::ConnectedState) {
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        systemLogBrowser->append(currentTime + ": Error: Not connected to the server.");
        return;
    }

    if(textToSendEdit->text().length() > 100) {
        systemLogBrowser->append("Error: Your message exceeds the 100 character limit.");
        return;
    }

    QString message = QString("M:%1:%2").arg(clientIdEdit->text(), textToSendEdit->text());
    clientSocket->write(message.toUtf8());
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    systemLogBrowser->append(currentTime + ": " + textToSendEdit->text());
}


void Client::onDisconnectButtonClicked() {
    QString disconnectMessage = QString("D:%1:").arg(clientIdEdit->text());
    clientSocket->write(disconnectMessage.toUtf8());
    clientSocket->disconnectFromHost();
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
    systemLogBrowser->append(currentTime + ": Disconnected from the server " + serverAddressEdit->text());
}

void Client::readFromServer() {
    QString data = clientSocket->readAll();
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");

    if (data.startsWith("Error: Client ID already use.")) {
        systemLogBrowser->append(currentTime + ": " + data);
        clientSocket->disconnectFromHost();
    } else if (data.startsWith("Connection successful")) {
        systemLogBrowser->append(currentTime + ": Connected to the server " + serverAddressEdit->text());
        messageReceivedBrowser->append(currentTime + ": Client:" + clientIdEdit->text() + " connected to server " + serverAddressEdit->text());
    } else {
        messageReceivedBrowser->append(data);
    }
}
