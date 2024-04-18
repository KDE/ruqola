/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "messagequeue.h"
using namespace Qt::Literals::StringLiterals;

#include "config-ruqola.h"
#include "ddpapi/ddpclient.h"
#include "rocketchataccount.h"
#include "ruqola.h"

#include "authenticationmanager.h"
#include "authenticationmanager/ddpauthenticationmanager.h"
#include "authenticationmanager/restauthenticationmanager.h"
#include "connection.h"

#include <QCborValue>
#include <QJsonArray>
#include <QPair>

MessageQueue::MessageQueue(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
}

MessageQueue::~MessageQueue()
{
}

void MessageQueue::loadCache()
{
    if (Ruqola::self()->useRestApiLogin()) {
        connect(mRocketChatAccount->restApi()->authenticationManager(),
                &RESTAuthenticationManager::loginStatusChanged,
                this,
                &MessageQueue::onLoginStatusChanged);
    } else {
        connect(mRocketChatAccount->ddp()->authenticationManager(), &DDPAuthenticationManager::loginStatusChanged, this, &MessageQueue::onLoginStatusChanged);
    }
}

QPair<QString, QJsonDocument> MessageQueue::fromJson(const QJsonObject &object)
{
    QPair<QString, QJsonDocument> pair;

    pair.first = object["method"_L1].toString();
    QJsonArray arr = object["params"_L1].toArray();
    pair.second = QJsonDocument(arr);
    return pair;
}

QByteArray MessageQueue::serialize(const QPair<QString, QJsonDocument> &pair)
{
    QJsonObject o;

    o["method"_L1] = QJsonValue(pair.first);

    QJsonArray arr;
    if (pair.second.isArray()) {
        arr.append(pair.second.array());
    } else if (pair.second.isObject()) {
        arr.append(pair.second.object());
    }

    o["params"_L1] = QJsonValue(arr);

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
    while (mRocketChatAccount->loginStatus() == AuthenticationManager::LoggedIn && !mRocketChatAccount->ddp()->messageQueue().empty()) {
        const QPair<QString, QJsonDocument> pair = mRocketChatAccount->ddp()->messageQueue().head();
        const QString method = pair.first;
        const QJsonDocument params = pair.second;
        mRocketChatAccount->ddp()->method(method, params);
    }
}

#include "moc_messagequeue.cpp"
