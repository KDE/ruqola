/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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

#include "ruqolawebsocket.h"
#include "ruqola_debug.h"

#include <QWebSocket>

RuqolaWebSocket::RuqolaWebSocket(QObject *parent)
    : AbstractWebSocket(parent)
{
    mWebSocket = new QWebSocket;
    connect(mWebSocket, &QWebSocket::connected, this, &RuqolaWebSocket::connected);
    connect(mWebSocket, &QWebSocket::disconnected, this, &RuqolaWebSocket::disconnected);
    connect(mWebSocket, &QWebSocket::textMessageReceived, this, &RuqolaWebSocket::textMessageReceived);
    connect(mWebSocket, &QWebSocket::sslErrors, this, &RuqolaWebSocket::sslErrors);
}

RuqolaWebSocket::~RuqolaWebSocket()
{
    delete mWebSocket;
    mWebSocket = nullptr;
}

void RuqolaWebSocket::openUrl(const QUrl &url)
{
    mWebSocket->open(url);
}

qint64 RuqolaWebSocket::sendTextMessage(const QString &message)
{
    qCDebug(RUQOLA_LOG) << " qint64 RuqolaWebSocket::sendTextMessage(const QString &message)"<<message;
    return mWebSocket->sendTextMessage(message);
}

bool RuqolaWebSocket::isValid() const
{
    return mWebSocket->isValid();
}

void RuqolaWebSocket::flush()
{
    mWebSocket->flush();
}

void RuqolaWebSocket::close()
{
    mWebSocket->close();
}

QAbstractSocket::SocketError RuqolaWebSocket::error() const
{
    return mWebSocket->error();
}

QUrl RuqolaWebSocket::requestUrl() const
{
    return mWebSocket->requestUrl();
}

QWebSocketProtocol::CloseCode RuqolaWebSocket::closeCode() const
{
    return mWebSocket->closeCode();
}

QString RuqolaWebSocket::closeReason() const
{
    return mWebSocket->closeReason();
}

qint64 RuqolaWebSocket::sendBinaryMessage(const QByteArray &data)
{
    return mWebSocket->sendBinaryMessage(data);
}

void RuqolaWebSocket::ignoreSslErrors()
{
    mWebSocket->ignoreSslErrors();
}
