/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commonmessagemodel.h"
#include "rocketchataccount.h"

#include "listmessages.h"

CommonMessageModel::CommonMessageModel(RocketChatAccount *account, QObject *parent)
    : MessagesModel(QStringLiteral("no_room"), account, nullptr, parent)
{
}

CommonMessageModel::~CommonMessageModel() = default;

void CommonMessageModel::clearModel()
{
    mStringNotFound = true;
    mLoadingInProgress = false;
    clear();
}

void CommonMessageModel::parse(const QJsonObject &obj)
{
    clear();
    ListMessages messages;
    messages.parseMessages(obj, QStringLiteral("messages"));
    mTotal = messages.total();
    addMessages(messages.listMessages());
    setStringNotFound(rowCount() == 0);
}

void CommonMessageModel::setStringNotFound(bool stringNotFound)
{
    if (mStringNotFound != stringNotFound) {
        mStringNotFound = stringNotFound;
        Q_EMIT stringNotFoundChanged();
    }
}

bool CommonMessageModel::loadSearchMessageInProgress() const
{
    return mLoadingInProgress;
}

void CommonMessageModel::setLoadSearchMessageInProgress(bool loadSearchMessageInProgress)
{
    if (mLoadingInProgress != loadSearchMessageInProgress) {
        mLoadingInProgress = loadSearchMessageInProgress;
        Q_EMIT loadingInProgressChanged();
    }
}

#include "moc_commonmessagemodel.cpp"
