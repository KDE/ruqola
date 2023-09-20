/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationconsolewidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QVBoxLayout>

AdministratorModerationConsoleWidget::AdministratorModerationConsoleWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

AdministratorModerationConsoleWidget::~AdministratorModerationConsoleWidget() = default;

#include "moc_administratormoderationconsolewidget.cpp"
