/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationoauthwidget.h"
#include "authenticationbutton.h"
#include "authenticationinfo.h"
#include <QVBoxLayout>

AuthenticationOauthWidget::AuthenticationOauthWidget(QWidget *parent)
    : QWidget{parent}
    , mMainLayout(new QVBoxLayout(this))
{
    mMainLayout->setObjectName(QStringLiteral("mainLayout"));
    mMainLayout->setContentsMargins({});
    setVisible(false);
    mMainLayout->addStretch(1);
}

AuthenticationOauthWidget::~AuthenticationOauthWidget() = default;

void AuthenticationOauthWidget::addAuthenticationMethod(const AuthenticationInfo &info)
{
    auto button = new AuthenticationButton(this);
    button->setObjectName(QStringLiteral("button_%1").arg(info.name()));
    connect(button, &AuthenticationButton::authentication, this, &AuthenticationOauthWidget::authentication);
    button->setAuthenticationInfo(info);
    mMainLayout->addWidget(button);
    mMainLayout->addStretch(1);
}

#include "moc_authenticationoauthwidget.cpp"
