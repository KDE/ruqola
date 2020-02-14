/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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
#include "searchchannel/searchchanneldelegate.h"
#include "model/searchchannelfilterproxymodel.h"
#include "model/searchchannelmodel.h"
#include "ruqolawidgets_debug.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include <QVBoxLayout>
#include <KLocalizedString>
#include <KLineEdit>
#include <QListWidget>

SearchChannelWidget::SearchChannelWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mSearchLineEdit = new KLineEdit(this);
    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setClearButtonEnabled(true);
    mSearchLineEdit->setTrapReturnKey(true);
    connect(mSearchLineEdit, &KLineEdit::textChanged, this, &SearchChannelWidget::slotTextChanged);
    mainLayout->addWidget(mSearchLineEdit);

    mResultListWidget = new QListView(this);
    mResultListWidget->setModel(Ruqola::self()->rocketChatAccount()->searchChannelFilterProxyModel());
    mResultListWidget->setObjectName(QStringLiteral("mResultListWidget"));
    mainLayout->addWidget(mResultListWidget);
    mResultListWidget->setItemDelegate(new SearchChannelDelegate(this));
    connect(mResultListWidget, &QListView::doubleClicked, this, &SearchChannelWidget::slotOpenChannel);
}

SearchChannelWidget::~SearchChannelWidget()
{
}

void SearchChannelWidget::slotTextChanged(const QString &str)
{
    Ruqola::self()->rocketChatAccount()->channelAndPrivateAutocomplete(str);
}

void SearchChannelWidget::slotOpenChannel(const QModelIndex &index)
{
    if (index.isValid()) {
        const auto channelType = index.data(SearchChannelModel::ChannelType).value<Channel::ChannelType>();
        const QString channelId = index.data(SearchChannelModel::ChannelId).toString();
        if (channelType == Channel::ChannelType::Room) {
            Ruqola::self()->rocketChatAccount()->openChannel(channelId, RocketChatAccount::ChannelTypeInfo::RoomId);
        } else if (channelType == Channel::ChannelType::PrivateChannel) {
            Ruqola::self()->rocketChatAccount()->openDirectChannel(channelId);
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << "Unknown open channel type : "  << channelType << " channelid : "  << channelId;
        }
    }
}
