/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef FAKEWEBSOCKET_H
#define FAKEWEBSOCKET_H

#include "../src/abstractwebsocket.h"

#include <QAbstractSocket>
#include <QUrl>

class FakeWebSocket : public AbstractWebSocket
{
    Q_OBJECT
public:
    explicit FakeWebSocket(QObject *parent = nullptr);
    ~FakeWebSocket();

    void forceSendTextMessage(const QString &message);
    void forceSendBinaryMessage(const QByteArray &message);

    void openUrl(const QUrl &url) override;
    qint64 sendTextMessage(const QString &message) override;
    bool isValid() const override;
    void flush() override;
    void close() override;
    QAbstractSocket::SocketError error() const override;
    QUrl requestUrl() const override;
    QWebSocketProtocol::CloseCode closeCode() const override;
    QString closeReason() const override;
    qint64 sendBinaryMessage(const QByteArray &data) override;
    void ignoreSslErrors() override;
};

#endif // FAKEWEBSOCKET_H
