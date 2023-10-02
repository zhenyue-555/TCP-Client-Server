#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMap>

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);

private slots:
    void newConnection();
    void clientDisconnected();
    void readFromClient();

private:
    QMap<QTcpSocket*, QString> clientMap;
    QTcpServer* serverSocket;
    QLabel* currentAddressLabel;
    QLabel* currentPortLabel;
    QListWidget* connectedClientsList;
    quint16 blockSize = 0;
};
