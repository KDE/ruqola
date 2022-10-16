/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "messagequeue.h"
#include "ddpapi/ddpauthenticationmanager.h"
#include "ddpapi/ddpclient.h"
#include "rocketchataccount.h"

#include <QCborValue>
#include <QDataStream>
#include <QDir>
#include <QJsonArray>
#include <QPair>

MessageQueue::MessageQueue(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
}

MessageQueue::~MessageQueue()
{
#if 0
    QDir cacheDir(mRocketChatAccount->ddp()->cachePath());
    qCDebug(RUQOLA_LOG) << "Caching Unsent messages to... " << cacheDir.path();
    if (!cacheDir.exists(cacheDir.path())) {
        if (!cacheDir.mkpath(cacheDir.path())) {
            qCWarning(RUQOLA_LOG) << "Problem for creating cachedir " << cacheDir;
        }
    }
    QFile f(cacheDir.absoluteFilePath(QStringLiteral("QueueCache")));
    if (f.open(QIODevice::WriteOnly)) {
        QDataStream out(&f);
        QQueue<QPair<QString, QJsonDocument> > queue = mRocketChatAccount->ddp()->messageQueue();
        for (QQueue<QPair<QString, QJsonDocument> >::iterator it = queue.begin(), end = queue.end(); it != end; ++it) {
            const QPair<QString, QJsonDocument> pair = *it;
            const QByteArray ba = serialize(pair);
            out.writeBytes(ba.constData(), ba.size());
        }
    }
#endif
}

void MessageQueue::loadCache()
{
    connect(mRocketChatAccount->ddp()->authenticationManager(), &DDPAuthenticationManager::loginStatusChanged, this, &MessageQueue::onLoginStatusChanged);
#if 0
    QDir cacheDir(mRocketChatAccount->ddp()->cachePath());
    // load unsent messages cache
    if (QFile::exists(cacheDir.absoluteFilePath(QStringLiteral("QueueCache")))) {
        QFile f(cacheDir.absoluteFilePath(QStringLiteral("QueueCache")));
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f);
            while (!f.atEnd()) {
                char *byteArray;
                quint32 length;
                in.readBytes(byteArray, length);
                const QByteArray ba = QByteArray::fromRawData(byteArray, length);
                QPair<QString, QJsonDocument> pair = MessageQueue::fromJson(QJsonDocument::fromBinaryData(ba).object());

                QString method = pair.first;
                QJsonDocument params = pair.second;
                mRocketChatAccount->ddp()->messageQueue().enqueue(qMakePair(method, params));
            }
        }
    }
#endif
}

QPair<QString, QJsonDocument> MessageQueue::fromJson(const QJsonObject &object)
{
    QPair<QString, QJsonDocument> pair;

    pair.first = object[QStringLiteral("method")].toString();
    QJsonArray arr = object[QStringLiteral("params")].toArray();
    pair.second = QJsonDocument(arr);
    return pair;
}

QByteArray MessageQueue::serialize(const QPair<QString, QJsonDocument> &pair)
{
    QJsonObject o;

    o[QStringLiteral("method")] = QJsonValue(pair.first);

    QJsonArray arr;
    if (pair.second.isArray()) {
        arr.append(pair.second.array());
    } else if (pair.second.isObject()) {
        arr.append(pair.second.object());
    }

    o[QStringLiteral("params")] = QJsonValue(arr);

    return QCborValue::fromJsonValue(o).toCbor();
}

void MessageQueue::onLoginStatusChanged()
{
    // retry sending messages
    // TODO: check login status?
    processQueue();
}

void MessageQueue::processQueue()
{
    // can be optimized using single shot timer
    while (mRocketChatAccount->loginStatus() == DDPAuthenticationManager::LoggedIn && !mRocketChatAccount->ddp()->messageQueue().empty()) {
        const QPair<QString, QJsonDocument> pair = mRocketChatAccount->ddp()->messageQueue().head();
        const QString method = pair.first;
        const QJsonDocument params = pair.second;
        mRocketChatAccount->ddp()->method(method, params);
    }
}
