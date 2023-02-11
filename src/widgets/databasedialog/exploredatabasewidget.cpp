/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasewidget.h"
#include "rocketchataccount.h"
#include <QVBoxLayout>

ExploreDatabaseWidget::ExploreDatabaseWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
{
}

ExploreDatabaseWidget::~ExploreDatabaseWidget() = default;
