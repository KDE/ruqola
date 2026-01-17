/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorycontainerwidget.h"

#include "directorystackedwidget.h"
#include "rocketchataccount.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
DirectoryContainerWidget::DirectoryContainerWidget(RocketChatAccount *account, DirectoryWidget::DirectoryType type, QWidget *parent)
    : QWidget{parent}
    , mDirectoryStackedWidget(new DirectoryStackedWidget(account, type, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(u"mainLayout"_s);

    mDirectoryStackedWidget->setObjectName(u"mDirectoryStackedWidget"_s);
    mainLayout->addWidget(mDirectoryStackedWidget);

    switch (type) {
    case DirectoryWidget::DirectoryType::Room:
    case DirectoryWidget::DirectoryType::Team: {
        if (account && !account->hasPermission(u"view-c-room"_s)) {
            mDirectoryStackedWidget->setIsAutorized(false);
        }
        break;
    }
    case DirectoryWidget::DirectoryType::User: {
        if (account && (!account->hasPermission(u"view-outside-room"_s) || !account->hasPermission(u"view-d-room"_s))) {
            mDirectoryStackedWidget->setIsAutorized(false);
        }
        break;
    }
    case DirectoryWidget::DirectoryType::Unknown:
        break;
    }

    connect(mDirectoryStackedWidget, &DirectoryStackedWidget::updateJoinButton, this, &DirectoryContainerWidget::updateJoinButton);
}

DirectoryContainerWidget::~DirectoryContainerWidget() = default;

void DirectoryContainerWidget::slotJoin()
{
    mDirectoryStackedWidget->slotJoin();
}

#include "moc_directorycontainerwidget.cpp"
