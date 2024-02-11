/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "channelsearchnamelineedit.h"
#include "common/completionlistview.h"
#include "common/userandchannelcompletiondelegate.h"
#include "connection.h"
#include "model/channelcompleterfilterproxymodel.h"
#include "model/inputcompletermodel.h"
#include "rocketchataccount.h"
#include "rooms/roomsautocompletechannelandprivatejob.h"
#include "ruqolawidgets_debug.h"
#include <KLineEditEventHandler>
#include <QJsonObject>

ChannelSearchNameLineEdit::ChannelSearchNameLineEdit(RocketChatAccount *account, QWidget *parent)
    : CompletionLineEdit(parent)
    , mChannelCompleterFilterProxyModel(new ChannelCompleterFilterProxyModel(this))
    , mChannelCompleterModel(new InputCompleterModel(account, this))
    , mRocketChatAccount(account)
{
    KLineEditEventHandler::catchReturnKey(this);
    mChannelCompleterFilterProxyModel->setSourceModel(mChannelCompleterModel);
    connect(this, &QLineEdit::textChanged, this, &ChannelSearchNameLineEdit::slotTextChanged);
    setCompletionModel(mChannelCompleterFilterProxyModel);
    connect(this, &ChannelSearchNameLineEdit::complete, this, &ChannelSearchNameLineEdit::slotComplete);

    auto userAndChannelCompletionDelegate = new UserAndChannelCompletionDelegate(mCompletionListView);
    userAndChannelCompletionDelegate->setRocketChatAccount(account);
    mCompletionListView->setItemDelegate(userAndChannelCompletionDelegate);

    mCompletionListView->setTextWidget(this);
}

ChannelSearchNameLineEdit::~ChannelSearchNameLineEdit() = default;

void ChannelSearchNameLineEdit::slotTextChanged(const QString &text)
{
    if (!text.trimmed().isEmpty()) {
        auto job = new RocketChatRestApi::RoomsAutocompleteChannelAndPrivateJob(this);
        RocketChatRestApi::RoomsAutocompleteChannelAndPrivateJob::RoomsAutocompleteChannelAndPrivateInfo info;
        info.name = text;
        job->setRoomsCompleterInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job,
                &RocketChatRestApi::RoomsAutocompleteChannelAndPrivateJob::roomsAutoCompleteChannelAndPrivateDone,
                this,
                &ChannelSearchNameLineEdit::slotSearchDone);
        if (!job->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start RoomsAutocompleteChannelAndPrivateJob";
        }
    } else {
        mChannelCompleterModel->clear();
        mCompletionListView->hide();
    }
}

void ChannelSearchNameLineEdit::slotSearchDone(const QJsonObject &obj)
{
    mChannelCompleterModel->parseSearchChannels(obj);
}

void ChannelSearchNameLineEdit::slotComplete(const QModelIndex &index)
{
    const QString completerName = index.data(InputCompleterModel::CompleterName).toString();
    const QString roomId = index.data(InputCompleterModel::Identifier).toString();
    ChannelCompletionInfo info;
    info.channelName = completerName;
    info.channelId = roomId;
    mCompletionListView->hide();
    disconnect(this, &QLineEdit::textChanged, this, &ChannelSearchNameLineEdit::slotTextChanged);
    Q_EMIT newRoomName(info);
    clear();
    connect(this, &QLineEdit::textChanged, this, &ChannelSearchNameLineEdit::slotTextChanged);
}

#include "moc_channelsearchnamelineedit.cpp"
