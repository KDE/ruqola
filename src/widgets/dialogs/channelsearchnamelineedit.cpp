/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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
#include "channelsearchnamelineedit.h"
#include "model/channelcompleterfilterproxymodel.h"
#include "model/channelcompletermodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"

ChannelSearchNameLineEdit::ChannelSearchNameLineEdit(QWidget *parent)
    : CompletionLineEdit(parent)
    , mChannelCompleterFilterProxyModel(new ChannelCompleterFilterProxyModel(this))
{
    mChannelCompleterFilterProxyModel->setSourceModel(new ChannelCompleterModel(this));
    connect(this, &QLineEdit::textChanged, this, &ChannelSearchNameLineEdit::slotTextChanged);
    setCompletionModel(mChannelCompleterFilterProxyModel);
    connect(this, &ChannelSearchNameLineEdit::complete, this, &ChannelSearchNameLineEdit::slotComplete);
}

ChannelSearchNameLineEdit::~ChannelSearchNameLineEdit()
{
}

void ChannelSearchNameLineEdit::slotTextChanged(const QString &text)
{
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    // TODO add exception!
    // Add current user + list of users already added.
    rcAccount->roomsAutocomplete(text, QString());
}

void ChannelSearchNameLineEdit::slotComplete(const QModelIndex &index)
{
#if 0
    const QString completerName = index.data(UserCompleterModel::UserName).toString();
    const QString userId = index.data(UserCompleterModel::UserId).toString();
    UserCompletionInfo info;
    info.username = completerName;
    info.userId = userId;
    mCompletionListView->hide();
    disconnect(this, &QLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotTextChanged);
    Q_EMIT newUserName(info);
    clear();
    connect(this, &QLineEdit::textChanged, this, &AddUsersCompletionLineEdit::slotTextChanged);
#endif
}
