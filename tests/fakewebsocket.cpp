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

#include "fakewebsocket.h"
#include <QDebug>

FakeWebSocket::FakeWebSocket(QObject *parent)
    : AbstractWebSocket(parent)
{
    qDebug() <<"FakeWebSocket::FakeWebSocket(QObject *parent) "<<this;
}

FakeWebSocket::~FakeWebSocket()
{
    qDebug() << "FakeWebSocket::~FakeWebSocket() " << this;
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
    qDebug() << " void FakeWebSocket::openUrl(const QUrl &url)"<<url;
    Q_EMIT connected();
}

qint64 FakeWebSocket::sendTextMessage(const QString &message)
{
    qDebug() << " FakeWebSocket::sendTextMessage(const QString &message)"<<message;
    return {};
}

bool FakeWebSocket::isValid() const
{
    qDebug() << "bool FakeWebSocket::isValid() const ";
    return true;
}

void FakeWebSocket::flush()
{
    qDebug() << " void FakeWebSocket::flush()";
}

void FakeWebSocket::close()
{
    qDebug() << " void FakeWebSocket::close()";
}

QAbstractSocket::SocketError FakeWebSocket::error() const
{
    qDebug() << " QAbstractSocket::SocketError FakeWebSocket::error() const";
    return {};
}

QUrl FakeWebSocket::requestUrl() const
{
    qDebug() << " QUrl FakeWebSocket::requestUrl() const";
    return {};
}

QWebSocketProtocol::CloseCode FakeWebSocket::closeCode() const
{
    qDebug() << "QWebSocketProtocol::CloseCode FakeWebSocket::closeCode() const ";
    return {};
}

QString FakeWebSocket::closeReason() const
{
    qDebug() << "QString FakeWebSocket::closeReason() const ";
    return {};
}

qint64 FakeWebSocket::sendBinaryMessage(const QByteArray &data)
{
    qDebug() << " qint64 FakeWebSocket::sendBinaryMessage(const QByteArray &data)"<<data;
    return {};
}

void FakeWebSocket::ignoreSslErrors()
{
    qDebug() << " void FakeWebSocket::ignoreSslErrors()";
}
