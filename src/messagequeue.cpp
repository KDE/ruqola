/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ruqola.h"
#include "ddpapi/ddpclient.h"
#include "ruqola_debug.h"
#include "messagequeue.h"
#include "rocketchataccount.h"

MessageQueue::MessageQueue(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
}

MessageQueue::~MessageQueue()
{
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
            out.writeBytes(ba, ba.size());
        }
    }
}

void MessageQueue::loadCache()
{
    connect(mRocketChatAccount->ddp(), &DDPClient::loginStatusChanged, this, &MessageQueue::onLoginStatusChanged);
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
                QByteArray ba = QByteArray::fromRawData(byteArray, length);
                QPair<QString, QJsonDocument> pair = MessageQueue::fromJson(QJsonDocument::fromBinaryData(ba).object());

                QString method = pair.first;
                QJsonDocument params = pair.second;
                mRocketChatAccount->ddp()->messageQueue().enqueue(qMakePair(method, params));
            }
        }
    }
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
    QJsonDocument d;
    QJsonObject o;

    o[QStringLiteral("method")] = QJsonValue(pair.first);

    QJsonArray arr;
    if (pair.second.isArray()) {
        arr.append(pair.second.array());
    } else if (pair.second.isObject()) {
        arr.append(pair.second.object());
    }

    o[QStringLiteral("params")] = QJsonValue(arr);

    d.setObject(o);
    return d.toBinaryData();
}

void MessageQueue::onLoginStatusChanged()
{
    //retry sending messages
    processQueue();
}

void MessageQueue::processQueue()
{
    //can be optimized using single shot timer
    while (mRocketChatAccount->loginStatus() == DDPClient::LoggedIn && !mRocketChatAccount->ddp()->messageQueue().empty()) {
        QPair<QString, QJsonDocument> pair = mRocketChatAccount->ddp()->messageQueue().head();
        QString method = pair.first;
        QJsonDocument params = pair.second;
        mRocketChatAccount->ddp()->method(method, params);
    }
}
