/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

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
    [[nodiscard]] bool isValid() const override;
    void flush() override;
    void close() override;
    [[nodiscard]] QAbstractSocket::SocketError error() const override;
    [[nodiscard]] QString errorString() const override;
    [[nodiscard]] QUrl requestUrl() const override;
    [[nodiscard]] QWebSocketProtocol::CloseCode closeCode() const override;
    [[nodiscard]] QString closeReason() const override;
    qint64 sendBinaryMessage(const QByteArray &data) override;
    void ignoreSslErrors() override;

private:
    void slotTextMessageReceived(const QString &msg);
    void slotError(QAbstractSocket::SocketError error);

private:
    RuqolaLogger *const mLogger;
    QWebSocket *const mWebSocket;
};
