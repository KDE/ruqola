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

#include "showlistmessagebasewidget.h"
#include "room/messagelistview.h"
#include "model/listmessagesmodelfilterproxymodel.h"

#include <QVBoxLayout>
#include <KLocalizedString>
#include <KLineEdit>
#include <QLabel>

ShowListMessageBaseWidget::ShowListMessageBaseWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mSearchMessageLineEdit = new KLineEdit(this);
    mSearchMessageLineEdit->setObjectName(QStringLiteral("mSearchMessageLineEdit"));
    mSearchMessageLineEdit->setClearButtonEnabled(true);
    mSearchMessageLineEdit->setTrapReturnKey(true);
    mainLayout->addWidget(mSearchMessageLineEdit);

    mMessageListInfo = new QLabel(this);
    mMessageListInfo->setObjectName(QStringLiteral("mMessageListInfo"));
    mainLayout->addWidget(mMessageListInfo);

    mMessageListView = new MessageListView(this);
    mMessageListView->setObjectName(QStringLiteral("mMessageListView"));
    mainLayout->addWidget(mMessageListView);
}

ShowListMessageBaseWidget::~ShowListMessageBaseWidget()
{
}

void ShowListMessageBaseWidget::setModel(ListMessagesModelFilterProxyModel *model)
{
    mMessageListView->setModel(model);
}
