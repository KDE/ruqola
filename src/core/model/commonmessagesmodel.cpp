/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commonmessagesmodel.h"
#include "rocketchataccount.h"

#include "listmessages.h"

CommonMessagesModel::CommonMessagesModel(RocketChatAccount *account, QObject *parent)
    : MessagesModel(QByteArrayLiteral("no_room"), account, nullptr, parent)
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
    messages.parseMessages(obj, QStringLiteral("messages"));
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
