/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractwebsocket.h"

class QWebSocket;
class RuqolaLogger;
class RuqolaWebSocket : public AbstractWebSocket
{
    Q_OBJECT
public:
    explicit RuqolaWebSocket(RuqolaLogger *logger, QObject *parent = nullptr);
    ~RuqolaWebSocket() override;

    void openUrl(const QUrl &url) override;
    qint64 sendTextMessage(const QString &message) override;
    Q_REQUIRED_RESULT bool isValid() const override;
    void flush() override;
    void close() override;
    QAbstractSocket::SocketError error() const override;
    Q_REQUIRED_RESULT QUrl requestUrl() const override;
    QWebSocketProtocol::CloseCode closeCode() const override;
    Q_REQUIRED_RESULT QString closeReason() const override;
    qint64 sendBinaryMessage(const QByteArray &data) override;
    void ignoreSslErrors() override;

private Q_SLOTS:
    void slotTextMessageReceived(const QString &msg);
    void slotError(QAbstractSocket::SocketError error);

private:
    RuqolaLogger *const mLogger;
    QWebSocket *const mWebSocket;
};

