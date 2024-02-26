/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolaloginstackwidget.h"
#include "ruqolaloginwidget.h"

RuqolaLoginStackWidget::RuqolaLoginStackWidget(QWidget *parent)
    : QStackedWidget(parent)
    , mRuqolaLoginWidget(new RuqolaLoginWidget(this))
{
    addWidget(mRuqolaLoginWidget);
}

void RuqolaLoginStackWidget::setRocketChatAccount(RocketChatAccount *account)
{
    // TODO
}

void RuqolaLoginStackWidget::setLoginStatus(DDPAuthenticationManager::LoginStatus status)
{
    // TODO
}

void RuqolaLoginStackWidget::showError(const QString &text)
{
    // TODO
}

RuqolaLoginStackWidget::~RuqolaLoginStackWidget() = default;

#include "moc_ruqolaloginstackwidget.cpp"
