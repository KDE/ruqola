/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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
}

bool DirectoryStackedWidget::isAutorized() const
{
    return mIsAutorized;
}

void DirectoryStackedWidget::setIsAutorized(bool newIsAutorized)
{
    mIsAutorized = newIsAutorized;
}

DirectoryStackedWidget::~DirectoryStackedWidget() = default;
