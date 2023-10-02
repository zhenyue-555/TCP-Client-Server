#include "server.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QHostAddress>
#include <QMessageBox>

Server::Server(QWidget *parent) : QWidget(parent) {
    serverSocket = new QTcpServer(this);

    // Set up the GUI
    QVBoxLayout *layout = new QVBoxLayout;

    connectedClientsList = new QListWidget(this);
    layout->addWidget(new QLabel("Connected Clients:"));
    layout->addWidget(connectedClientsList);

    if (!serverSocket->listen(QHostAddress("127.0.0.1"), 1502)) {
        QMessageBox::critical(this, "Error", "Failed to start the server: " + serverSocket->errorString());
        return;
    } else {
        layout->addWidget(new QLabel("Server started on address: 127.0.0.1, port: 1502"));
    }

    connect(serverSocket, &QTcpServer::newConnection, this, &Server::newConnection);

    setLayout(layout);
}

void Server::newConnection() {
    QTcpSocket *clientSocket = serverSocket->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);

    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::clientDisconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::readFromClient);
}

void Server::readFromClient() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if(!clientSocket) return;

    QString data = clientSocket->readAll();
    QStringList parts = data.split(":");

    if(parts.count() < 2) {clientSocket->write("Error: Malformed message received.");
        return;
    }

    QString messageType = parts[0];
    QString clientId = parts[1];
    QString payload = parts.count() > 2 ? parts[2] : "";
    if(payload.length() > 100) {
        QString errorResponse = "Error: Your message exceeds the 100 character limit.";
        clientSocket->write(errorResponse.toUtf8());
        return;
    }

    if (messageType == "C") {
        if (clientMap.contains(clientSocket) || clientMap.values().contains(clientId)) {
            QString errorResponse = "Error: Client ID already use.";
            clientSocket->write(errorResponse.toUtf8());
            clientSocket->disconnectFromHost();  // Optionally disconnect
            return;
        }
        clientMap[clientSocket] = clientId;
        connectedClientsList->addItem(clientId);

        // Send acknowledgment for successful connection
        clientSocket->write("Connection successful");
    }
    else if(messageType == "M") {
        // Get the current time
        QString currentTime = QTime::currentTime().toString("hh:mm:ss");

        // Format the response string
        QString response = QString("%1: %2 [ACK:%3]").arg(currentTime, payload, clientId);

        // Send the response to the client
        clientSocket->write(response.toUtf8());
    }
    else if(messageType == "D") {
        clientSocket->disconnectFromHost();
    }

}

void Server::clientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if(!clientSocket) return;

    QString clientId = clientMap.value(clientSocket, "");

    // Remove from the GUI
    QList<QListWidgetItem*> items = connectedClientsList->findItems(clientId, Qt::MatchExactly);
    foreach(QListWidgetItem* item, items) {
        delete connectedClientsList->takeItem(connectedClientsList->row(item));
    }

    // Remove from the clientMap
    clientMap.remove(clientSocket);
}
