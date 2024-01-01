/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolawebsocket.h"
#include "config-ruqola.h"
#include "ruqola_debug.h"
#include "ruqolalogger.h"

#include <QWebSocket>

RuqolaWebSocket::RuqolaWebSocket(RuqolaLogger *logger, QObject *parent)
    : AbstractWebSocket(parent)
    , mLogger(logger)
    , mWebSocket(new QWebSocket)
{
    connect(mWebSocket, &QWebSocket::connected, this, &RuqolaWebSocket::connected);
    connect(mWebSocket, &QWebSocket::disconnected, this, &RuqolaWebSocket::disconnected);
    connect(mWebSocket, &QWebSocket::textMessageReceived, this, &RuqolaWebSocket::slotTextMessageReceived);
    connect(mWebSocket, &QWebSocket::sslErrors, this, &RuqolaWebSocket::sslErrors);
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
    connect(mWebSocket, qOverload<QAbstractSocket::SocketError>(&QWebSocket::error), this, &RuqolaWebSocket::slotError);
#else
    connect(mWebSocket, &QWebSocket::errorOccurred, this, &RuqolaWebSocket::slotError);
#endif
}

RuqolaWebSocket::~RuqolaWebSocket()
{
    delete mWebSocket;
}

void RuqolaWebSocket::openUrl(const QUrl &url)
{
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QStringLiteral("webkit/Ruqola-%1 (%2 %3) webkit/Ruqola-%1")
                          .arg(QStringLiteral(RUQOLA_VERSION), QSysInfo::prettyProductName(), QSysInfo::currentCpuArchitecture()));
    mWebSocket->open(request);
}

qint64 RuqolaWebSocket::sendTextMessage(const QString &message)
{
    qCDebug(RUQOLA_LOG) << " qint64 RuqolaWebSocket::sendTextMessage(const QString &message)" << message;
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
