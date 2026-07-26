/*
   SPDX-FileCopyrightText: 2026 Till Adam <till.adam@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ddpclienttest.h"

#include <QJsonObject>

#include "abstractwebsocket.h"
#include "authenticationmanager/ddpauthenticationmanager.h"
#include "ddpapi/ddpclient.h"

#include <QTest>

class FakeWebSocket final : public AbstractWebSocket
{
public:
    using AbstractWebSocket::AbstractWebSocket;

    void openUrl(const QUrl &url) override
    {
        mRequestUrl = url;
    }

    [[nodiscard]] qint64 sendTextMessage(const QString &message) override
    {
        return message.size();
    }

    [[nodiscard]] bool isValid() const override
    {
        return false;
    }

    void flush() override
    {
    }

    void close() override
    {
    }

    [[nodiscard]] QAbstractSocket::SocketError error() const override
    {
        return QAbstractSocket::UnknownSocketError;
    }

    [[nodiscard]] QString errorString() const override
    {
        return {};
    }

    [[nodiscard]] QUrl requestUrl() const override
    {
        return mRequestUrl;
    }

    [[nodiscard]] QWebSocketProtocol::CloseCode closeCode() const override
    {
        return mCloseCode;
    }

    [[nodiscard]] QString closeReason() const override
    {
        return {};
    }

    void ignoreSslErrors() override
    {
    }

    [[nodiscard]] QWebSocketProtocol::Version version() const override
    {
        return QWebSocketProtocol::VersionLatest;
    }

    void setCloseCode(QWebSocketProtocol::CloseCode closeCode)
    {
        mCloseCode = closeCode;
    }

private:
    QUrl mRequestUrl;
    QWebSocketProtocol::CloseCode mCloseCode = QWebSocketProtocol::CloseCodeNormal;
};

QTEST_GUILESS_MAIN(DDPClientTest)

void DDPClientTest::closeNotificationSurvivesAuthenticationCleanup_data()
{
    QTest::addColumn<int>("closeCode");
    QTest::addColumn<bool>("normalClose");

    QTest::addRow("normal") << static_cast<int>(QWebSocketProtocol::CloseCodeNormal) << true;
    QTest::addRow("unexpected") << static_cast<int>(QWebSocketProtocol::CloseCodeAbnormalDisconnection) << false;
}

void DDPClientTest::closeNotificationSurvivesAuthenticationCleanup()
{
    QFETCH(int, closeCode);
    QFETCH(bool, normalClose);

    DDPClient client;
    client.setDDPClientAccountParameter(new DDPClient::DDPClientAccountParameter);
    auto webSocket = new FakeWebSocket;
    client.mWebSocket = webSocket;

    QObject account;
    bool reconnectScheduled = false;
    bool authenticationCleanupRan = false;
    const auto queueReconnect = [&account, &reconnectScheduled]() {
        QMetaObject::invokeMethod(
            &account,
            [&reconnectScheduled]() {
                reconnectScheduled = true;
            },
            Qt::QueuedConnection);
    };
    if (normalClose) {
        connect(&client, &DDPClient::disconnectedByServer, &account, queueReconnect);
    } else {
        connect(&client, &DDPClient::wsClosedSocketError, &account, queueReconnect);
    }

    connect(client.authenticationManager(), &DDPAuthenticationManager::loginStatusChanged, &account, [&client, &account, &authenticationCleanupRan]() {
        authenticationCleanupRan = true;
        disconnect(&client, nullptr, &account, nullptr);
    });

    webSocket->setCloseCode(static_cast<QWebSocketProtocol::CloseCode>(closeCode));
    client.onWSclosed();

    QVERIFY(authenticationCleanupRan);
    QTRY_VERIFY(reconnectScheduled);
}

#include "moc_ddpclienttest.cpp"
