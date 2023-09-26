/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagemodel.h"
#include "rocketchataccount.h"

#include "listmessages.h"

SearchMessageModel::SearchMessageModel(RocketChatAccount *account, QObject *parent)
    : MessageModel(QStringLiteral("no_room"), account, nullptr, parent)
{
}

SearchMessageModel::~SearchMessageModel() = default;

void SearchMessageModel::clearModel()
{
    mStringNotFound = true;
    mLoadingInProgress = false;
    clear();
}

void SearchMessageModel::parse(const QJsonObject &obj)
{
    clear();
    ListMessages messages;
    messages.parseMessages(obj, QStringLiteral("messages"));
    mTotal = messages.total();
    addMessages(messages.listMessages());
    setStringNotFound(rowCount() == 0);
}

void SearchMessageModel::setStringNotFound(bool stringNotFound)
{
    if (mStringNotFound != stringNotFound) {
        mStringNotFound = stringNotFound;
        Q_EMIT stringNotFoundChanged();
    }
}

bool SearchMessageModel::loadSearchMessageInProgress() const
{
    return mLoadingInProgress;
}

void SearchMessageModel::setLoadSearchMessageInProgress(bool loadSearchMessageInProgress)
{
    if (mLoadingInProgress != loadSearchMessageInProgress) {
        mLoadingInProgress = loadSearchMessageInProgress;
        Q_EMIT loadingInProgressChanged();
    }
}

bool SearchMessageModel::stringNotFound() const
{
    return mStringNotFound;
}

#include "moc_searchmessagemodel.cpp"
