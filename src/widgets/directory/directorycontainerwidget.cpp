/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorycontainerwidget.h"
#include "directorystackedwidget.h"
#include "rocketchataccount.h"
#include <QVBoxLayout>

DirectoryContainerWidget::DirectoryContainerWidget(RocketChatAccount *account, DirectoryWidget::DirectoryType type, QWidget *parent)
    : QWidget{parent}
    , mDirectoryStackedWidget(new DirectoryStackedWidget(account, type, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mDirectoryStackedWidget->setObjectName(QStringLiteral("mDirectoryStackedWidget"));
    mainLayout->addWidget(mDirectoryStackedWidget);

    switch (type) {
    case DirectoryWidget::DirectoryType::Room:
    case DirectoryWidget::DirectoryType::Team: {
        if (account && !account->hasPermission(QStringLiteral("view-c-room"))) {
            mDirectoryStackedWidget->setIsAutorized(false);
        }
        break;
    }
    case DirectoryWidget::DirectoryType::User: {
        if (account && (!account->hasPermission(QStringLiteral("view-outside-room")) || !account->hasPermission(QStringLiteral("view-d-room")))) {
            mDirectoryStackedWidget->setIsAutorized(false);
        }
        break;
    }
    case DirectoryWidget::Unknown:
        break;
    }
}

DirectoryContainerWidget::~DirectoryContainerWidget() = default;

void DirectoryContainerWidget::fill()
{
    mDirectoryStackedWidget->fillDirectory();
}

#include "moc_directorycontainerwidget.cpp"
