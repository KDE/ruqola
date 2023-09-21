/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QAbstractSocket>
#include <QObject>
#include <QSslError>
#include <QWebSocketProtocol>
class LIBRUQOLACORE_TESTS_EXPORT AbstractWebSocket : public QObject
{
    Q_OBJECT
public:
    explicit AbstractWebSocket(QObject *parent = nullptr);
    ~AbstractWebSocket() override;

    virtual void openUrl(const QUrl &url) = 0;
    [[nodiscard]] virtual qint64 sendTextMessage(const QString &message) = 0;
    virtual bool isValid() const = 0;
    virtual void flush() = 0;
    virtual void close() = 0;
    [[nodiscard]] virtual QAbstractSocket::SocketError error() const = 0;
    [[nodiscard]] virtual QString errorString() const = 0;
    [[nodiscard]] virtual QUrl requestUrl() const = 0;
    [[nodiscard]] virtual QWebSocketProtocol::CloseCode closeCode() const = 0;
    [[nodiscard]] virtual QString closeReason() const = 0;
    virtual qint64 sendBinaryMessage(const QByteArray &data) = 0;
    virtual void ignoreSslErrors() = 0;

Q_SIGNALS:
    void disconnected();
    void connected();
    void textMessageReceived(const QString &message);
    void sslErrors(const QList<QSslError> &errors);
    void socketError(QAbstractSocket::SocketError error, const QString &errorString);

private:
    Q_DISABLE_COPY(AbstractWebSocket)
};
