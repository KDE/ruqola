/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolawebsocket.h"
using namespace Qt::Literals::StringLiterals;

#include "config-ruqola.h"
#include "ruqola_debug.h"
#include "ruqola_reconnect_core_debug.h"
#include "ruqolalogger.h"

#include <QWebSocket>

RuqolaWebSocket::RuqolaWebSocket(RuqolaLogger *logger, QObject *parent)
    : AbstractWebSocket(parent)
    , mLogger(logger)
    , mWebSocket(new QWebSocket)
{
    connect(mWebSocket, &QWebSocket::connected, this, [this]() {
        qDebug(RUQOLA_RECONNECT_LOG) << "QWebSocket emitted connected";
        Q_EMIT connected();
    });
    connect(mWebSocket, &QWebSocket::disconnected, this, [this]() {
        qDebug(RUQOLA_RECONNECT_LOG) << "QWebSocket emitted disconnected";
        Q_EMIT disconnected();
    });
    connect(mWebSocket, &QWebSocket::textMessageReceived, this, &RuqolaWebSocket::slotTextMessageReceived);
    connect(mWebSocket, &QWebSocket::sslErrors, this, &RuqolaWebSocket::sslErrors);
    connect(mWebSocket, &QWebSocket::errorOccurred, this, &RuqolaWebSocket::slotError);
}

RuqolaWebSocket::~RuqolaWebSocket()
{
    delete mWebSocket;
}

void RuqolaWebSocket::openUrl(const QUrl &url)
{
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(
        QNetworkRequest::UserAgentHeader,
        u"webkit/Ruqola-%1 (%2 %3) webkit/Ruqola-%1"_s.arg(QStringLiteral(RUQOLA_VERSION), QSysInfo::prettyProductName(), QSysInfo::currentCpuArchitecture()));
    mWebSocket->open(request);
}

qint64 RuqolaWebSocket::sendTextMessage(const QString &message)
{
    qCDebug(RUQOLA_LOG) << "RuqolaWebSocket::sendTextMessage" << message;
    if (mLogger) {
        mLogger->dataSent(message.toUtf8());
    }
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

QString RuqolaWebSocket::errorString() const
{
    return mWebSocket->errorString();
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

QWebSocketProtocol::Version RuqolaWebSocket::version() const
{
    return mWebSocket->version();
}

void RuqolaWebSocket::slotTextMessageReceived(const QString &msg)
{
    if (mLogger) {
        mLogger->dataReceived(msg.toUtf8());
    }
    Q_EMIT textMessageReceived(msg);
}

void RuqolaWebSocket::slotError(QAbstractSocket::SocketError error)
{
    Q_EMIT socketError(error, mWebSocket->errorString());
}

#include "moc_ruqolawebsocket.cpp"
