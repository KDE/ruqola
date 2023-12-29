/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationoauthwidget.h"
#include "authenticationbutton.h"
#include <QVBoxLayout>

AuthenticationOauthWidget::AuthenticationOauthWidget(QWidget *parent)
    : QWidget{parent}
    , mMainLayout(new QVBoxLayout)
{
    auto topLayout = new QHBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("topLayout"));
    topLayout->setContentsMargins({});

    mMainLayout->setObjectName(QStringLiteral("mainLayout"));
    mMainLayout->setContentsMargins({});
    topLayout->addStretch(0);
    topLayout->addLayout(mMainLayout);
    topLayout->addStretch(0);
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
}

#include "moc_authenticationoauthwidget.cpp"
