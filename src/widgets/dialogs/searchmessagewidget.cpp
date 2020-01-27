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

#include "ruqola.h"
#include "rocketchataccount.h"
#include "searchmessagewidget.h"
#include "room/messagelistview.h"
#include "model/searchmessagefilterproxymodel.h"
#include <KLineEdit>
#include <KLocalizedString>
#include <QListWidget>
#include <QVBoxLayout>

SearchMessageWidget::SearchMessageWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mSearchLineEdit = new KLineEdit(this);
    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setClearButtonEnabled(true);
    mSearchLineEdit->setTrapReturnKey(true);
    mainLayout->addWidget(mSearchLineEdit);

    mResultListWidget = new MessageListView(this);
    mResultListWidget->setObjectName(QStringLiteral("mResultListWidget"));
    mainLayout->addWidget(mResultListWidget);
    connect(mSearchLineEdit, &QLineEdit::returnPressed, this, &SearchMessageWidget::slotSearchMessages);
    //TODO Laurent we need to fix searchMessageFilterProxyModel first mResultListWidget->setModel(Ruqola::self()->rocketChatAccount()->searchMessageFilterProxyModel());
}

SearchMessageWidget::~SearchMessageWidget()
{
}

void SearchMessageWidget::slotSearchMessages()
{
    Ruqola::self()->rocketChatAccount()->messageSearch(mSearchLineEdit->text(), mRoomId);
}

QString SearchMessageWidget::roomId() const
{
    return mRoomId;
}

void SearchMessageWidget::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}
