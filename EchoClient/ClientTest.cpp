#include <QtTest/QtTest>
#include "client.h"

class ClientTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testSendMessages();

private:
    Client *client1, *client2, *client3;
};

void ClientTest::initTestCase() {
    client1 = new Client();
    client2 = new Client();
    client3 = new Client();
}

void ClientTest::cleanupTestCase() {
    delete client1;
    delete client2;
    delete client3;
}

void ClientTest::testSendMessages() {
    // Simulate entering text for client IDs
    QTest::keyClicks(client1->findChild<QLineEdit*>(), "1");
    QTest::keyClicks(client2->findChild<QLineEdit*>(), "2");
    QTest::keyClicks(client3->findChild<QLineEdit*>(), "3");

    // Simulate pressing the Connect button for each client
    QTest::mouseClick(client1->findChild<QPushButton*>("Connect"), Qt::LeftButton);
    QTest::mouseClick(client2->findChild<QPushButton*>("Connect"), Qt::LeftButton);
    QTest::mouseClick(client3->findChild<QPushButton*>("Connect"), Qt::LeftButton);

    // Assume it takes a maximum of 5 seconds to connect
    QTest::qWait(5000);

    // Simulate entering text to send from each client
    QTest::keyClicks(client1->findChild<QLineEdit*>(), "Message from Client 1");
    QTest::keyClicks(client2->findChild<QLineEdit*>(), "Message from Client 2");
    QTest::keyClicks(client3->findChild<QLineEdit*>(), "Message from Client 3");

    // Simulate pressing the Send button for each client
    QTest::mouseClick(client1->findChild<QPushButton*>("Send"), Qt::LeftButton);
    QTest::mouseClick(client2->findChild<QPushButton*>("Send"), Qt::LeftButton);
    QTest::mouseClick(client3->findChild<QPushButton*>("Send"), Qt::LeftButton);

    // Give some time for messages to be processed. Adjust the wait time as needed.
    QTest::qWait(3000);
}

QTEST_MAIN(ClientTest)
#include "clienttest.moc"
