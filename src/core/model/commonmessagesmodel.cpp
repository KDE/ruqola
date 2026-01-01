/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commonmessagesmodel.h"
using namespace Qt::Literals::StringLiterals;

#include "rocketchataccount.h"

#include "listmessages.h"

CommonMessagesModel::CommonMessagesModel(RocketChatAccount *account, QObject *parent)
    : MessagesModel("no_room"_ba, account, nullptr, parent)
{
}

CommonMessagesModel::~CommonMessagesModel() = default;

void CommonMessagesModel::clearModel()
{
    mStringNotFound = true;
    mLoadingInProgress = false;
    clear();
}

QList<Message> CommonMessagesModel::extractMessages(const QJsonObject &obj)
{
    ListMessages messages;
    messages.parseMessages(obj, u"messages"_s);
    return messages.listMessages();
}

bool CommonMessagesModel::parse(const QJsonObject &obj, bool clearMessages, bool insertListMessages)
{
    if (clearMessages) {
        clear();
    }
    const QList<Message> messages = extractMessages(obj);
    const bool isEmpty = messages.isEmpty();
    if (!isEmpty) {
        addMessages(messages, insertListMessages);
    }
    setStringNotFound(rowCount() == 0);
    return isEmpty;
}

void CommonMessagesModel::setStringNotFound(bool stringNotFound)
{
    if (mStringNotFound != stringNotFound) {
        mStringNotFound = stringNotFound;
        Q_EMIT stringNotFoundChanged();
    }
}

bool CommonMessagesModel::loadCommonMessagesInProgress() const
{
    return mLoadingInProgress;
}

void CommonMessagesModel::setLoadCommonMessagesInProgress(bool loadSearchMessageInProgress)
{
    if (mLoadingInProgress != loadSearchMessageInProgress) {
        mLoadingInProgress = loadSearchMessageInProgress;
        Q_EMIT loadingInProgressChanged();
    }
}

#include "moc_commonmessagesmodel.cpp"
