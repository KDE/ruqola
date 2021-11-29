/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchchannelwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/searchchannelfilterproxymodel.h"
#include "model/searchchannelmodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "searchchannel/searchchanneldelegate.h"
#include "searchchannellistview.h"
#include <KLocalizedString>
#include <QLineEdit>
#include <QVBoxLayout>

SearchChannelWidget::SearchChannelWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchLineEdit(new SearchWithDelayLineEdit(this))
    , mResultListWidget(new SearchChannelListView(this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Channel..."));
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);
    connect(mSearchLineEdit, &QLineEdit::returnPressed, this, [this]() {
        slotTextChanged(mSearchLineEdit->text());
    });
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchRequested, this, &SearchChannelWidget::slotTextChanged);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchCleared, this, &SearchChannelWidget::slotSearchCleared);
    mainLayout->addWidget(mSearchLineEdit);

    if (mRocketChatAccount) {
        mResultListWidget->setModel(mRocketChatAccount->searchChannelFilterProxyModel());
    }
    mResultListWidget->setObjectName(QStringLiteral("mResultListWidget"));
    mainLayout->addWidget(mResultListWidget);
    auto searchDeletegate = new SearchChannelDelegate(this);
    mResultListWidget->setItemDelegate(searchDeletegate);
    connect(searchDeletegate, &SearchChannelDelegate::channelSelected, this, &SearchChannelWidget::slotOpenChannel);
    if (mRocketChatAccount) {
        mRocketChatAccount->channelAndPrivateAutocomplete(QString());
    }
}

SearchChannelWidget::~SearchChannelWidget() = default;

void SearchChannelWidget::slotSearchCleared()
{
    mRocketChatAccount->searchChannelModel()->clear();
    mResultListWidget->setSearchChannel(false);
}

void SearchChannelWidget::slotTextChanged(const QString &str)
{
    mRocketChatAccount->channelAndPrivateAutocomplete(str);
    mResultListWidget->setSearchChannel(!str.isEmpty());
}

void SearchChannelWidget::slotOpenChannel(const QModelIndex &index)
{
    if (index.isValid()) {
        const auto channelType = index.data(SearchChannelModel::ChannelType).value<Channel::ChannelType>();
        const QString channelId = index.data(SearchChannelModel::ChannelId).toString();
        if (channelType == Channel::ChannelType::Room) {
            mRocketChatAccount->openChannel(channelId, RocketChatAccount::ChannelTypeInfo::RoomId);
        } else if (channelType == Channel::ChannelType::DirectChannel) {
            mRocketChatAccount->openDirectChannel(channelId);
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << "Unknown open channel type : " << channelType << " channelid : " << channelId;
        }
    }
}
