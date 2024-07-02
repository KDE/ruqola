/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorystackedwidget.h"
#include "directorynotauthorizedwidget.h"

DirectoryStackedWidget::DirectoryStackedWidget(RocketChatAccount *account, DirectoryWidget::DirectoryType type, QWidget *parent)
    : QStackedWidget(parent)
    , mDirectoryWidget(new DirectoryWidget(account, type, this))
    , mDirectoryNotAutorizedWidget(new DirectoryNotAuthorizedWidget(this))
{
    mDirectoryWidget->setObjectName(QStringLiteral("mDirectoryWidget"));
    mDirectoryNotAutorizedWidget->setObjectName(QStringLiteral("mDirectoryNotAutorizedWidget"));

    addWidget(mDirectoryWidget);
    addWidget(mDirectoryNotAutorizedWidget);
    setCurrentWidget(mDirectoryWidget);
    connect(mDirectoryWidget, &DirectoryWidget::updateJoinButton, this, &DirectoryStackedWidget::updateJoinButton);
}

DirectoryStackedWidget::~DirectoryStackedWidget() = default;

bool DirectoryStackedWidget::isAutorized() const
{
    return mIsAutorized;
}

void DirectoryStackedWidget::setIsAutorized(bool newIsAutorized)
{
    mIsAutorized = newIsAutorized;
    if (!mIsAutorized) {
        setCurrentWidget(mDirectoryNotAutorizedWidget);
    }
}

void DirectoryStackedWidget::slotJoin()
{
    mDirectoryWidget->slotJoin();
}

DirectoryWidget *DirectoryStackedWidget::directoryWidget() const
{
    return mDirectoryWidget;
}

DirectoryNotAuthorizedWidget *DirectoryStackedWidget::directoryNotAutorizedWidget() const
{
    return mDirectoryNotAutorizedWidget;
}

#include "moc_directorystackedwidget.cpp"
