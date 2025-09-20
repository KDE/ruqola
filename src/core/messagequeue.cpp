/*

 * SPDX-FileCopyrightText: 2016 Riccardo Iaconelli <riccardo@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "messagequeue.h"

#include "ddpapi/ddpclient.h"
#include "rocketchataccount.h"
#include "ruqola.h"

#include "authenticationmanager.h"
#include "authenticationmanager/ddpauthenticationmanager.h"
#include "authenticationmanager/restauthenticationmanager.h"
#include "connection.h"

#include <QJsonArray>
#include <QPair>

using namespace Qt::Literals::StringLiterals;
MessageQueue::MessageQueue(RocketChatAccount *account, QObject *parent)
    : QObject(parent)
    , mRocketChatAccount(account)
{
}

MessageQueue::~MessageQueue() = default;

void MessageQueue::loadCache()
{
    if (Ruqola::useRestApiLogin()) {
        connect(mRocketChatAccount->restApi()->authenticationManager(),
                &RESTAuthenticationManager::loginStatusChanged,
                this,
                &MessageQueue::onLoginStatusChanged);
    } else {
        connect(mRocketChatAccount->ddp()->authenticationManager(), &DDPAuthenticationManager::loginStatusChanged, this, &MessageQueue::onLoginStatusChanged);
    }
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
        QJsonArray arr;
        if (pair.second.isArray()) {
            arr.append(pair.second.array());
        } else if (pair.second.isObject()) {
            arr.append(pair.second.object());
        }
        mRocketChatAccount->ddp()->method(method, arr);
    }
}

#include "moc_messagequeue.cpp"
