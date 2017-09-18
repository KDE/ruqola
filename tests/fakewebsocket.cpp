/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#include "fakewebsocket.h"
#include <QDebug>

FakeWebSocket::FakeWebSocket(QObject *parent)
    : AbstractWebSocket(parent)
{
    qDebug() <<"FakeWebSocket::FakeWebSocket(QObject *parent) "<<this;
}

void FakeWebSocket::forceSendTextMessage(const QString &message)
{
    sendTextMessage(message);
}

void FakeWebSocket::forceSendBinaryMessage(const QByteArray &message)
{
    sendBinaryMessage(message);
}

void FakeWebSocket::openUrl(const QUrl &url)
{
}

qint64 FakeWebSocket::sendTextMessage(const QString &message)
{
    return {};
}

bool FakeWebSocket::isValid() const
{
    return true;
}

void FakeWebSocket::flush()
{
}

void FakeWebSocket::close()
{
}

QAbstractSocket::SocketError FakeWebSocket::error() const
{
    return {};
}

QUrl FakeWebSocket::requestUrl() const
{
    return {};
}

QWebSocketProtocol::CloseCode FakeWebSocket::closeCode() const
{
    return {};
}

QString FakeWebSocket::closeReason() const
{
    return {};
}

qint64 FakeWebSocket::sendBinaryMessage(const QByteArray &data)
{
    return {};
}

void FakeWebSocket::ignoreSslErrors()
{
    qDebug() << " void FakeWebSocket::ignoreSslErrors()";
}
