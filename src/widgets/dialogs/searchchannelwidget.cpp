/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

SearchChannelWidget::SearchChannelWidget(QWidget *parent)
    : QWidget(parent)
    , mSearchLineEdit(new SearchWithDelayLineEdit(this))
    , mResultListWidget(new SearchChannelListView(this))
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

    mResultListWidget->setModel(Ruqola::self()->rocketChatAccount()->searchChannelFilterProxyModel());
    mResultListWidget->setObjectName(QStringLiteral("mResultListWidget"));
    mainLayout->addWidget(mResultListWidget);
    auto searchDeletegate = new SearchChannelDelegate(this);
    mResultListWidget->setItemDelegate(searchDeletegate);
    connect(searchDeletegate, &SearchChannelDelegate::channelSelected, this, &SearchChannelWidget::slotOpenChannel);
    Ruqola::self()->rocketChatAccount()->channelAndPrivateAutocomplete(QString());
}

SearchChannelWidget::~SearchChannelWidget()
{
}

void SearchChannelWidget::slotSearchCleared()
{
    Ruqola::self()->rocketChatAccount()->searchChannelModel()->clear();
    mResultListWidget->setSearchChannel(false);
}

void SearchChannelWidget::slotTextChanged(const QString &str)
{
    Ruqola::self()->rocketChatAccount()->channelAndPrivateAutocomplete(str);
    mResultListWidget->setSearchChannel(!str.isEmpty());
}

void SearchChannelWidget::slotOpenChannel(const QModelIndex &index)
{
    if (index.isValid()) {
        const auto channelType = index.data(SearchChannelModel::ChannelType).value<Channel::ChannelType>();
        const QString channelId = index.data(SearchChannelModel::ChannelId).toString();
        if (channelType == Channel::ChannelType::Room) {
            Ruqola::self()->rocketChatAccount()->openChannel(channelId, RocketChatAccount::ChannelTypeInfo::RoomId);
        } else if (channelType == Channel::ChannelType::DirectChannel) {
            Ruqola::self()->rocketChatAccount()->openDirectChannel(channelId);
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << "Unknown open channel type : " << channelType << " channelid : " << channelId;
        }
    }
}
