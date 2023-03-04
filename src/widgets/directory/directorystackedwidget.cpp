/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorystackedwidget.h"

DirectoryStackedWidget::DirectoryStackedWidget(RocketChatAccount *account, DirectoryWidget::DirectoryType type, QWidget *parent)
    : QStackedWidget(parent)
{
}

DirectoryStackedWidget::~DirectoryStackedWidget()
{
}
