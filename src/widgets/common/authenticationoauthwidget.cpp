/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "authenticationoauthwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "authenticationbutton.h"
#include <QVBoxLayout>

AuthenticationOauthWidget::AuthenticationOauthWidget(QWidget *parent)
    : QWidget{parent}
    , mMainLayout(new QVBoxLayout)
{
    auto topLayout = new QHBoxLayout(this);
    topLayout->setObjectName(u"topLayout"_s);
    topLayout->setContentsMargins({});

    mMainLayout->setObjectName(u"mainLayout"_s);
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
    button->setObjectName(u"button_%1"_s.arg(info.name()));
    connect(button, &AuthenticationButton::authentication, this, &AuthenticationOauthWidget::authentication);
    button->setAuthenticationInfo(info);
    mMainLayout->addWidget(button);
    mAuthenticationButtonList.append(button);
}

void AuthenticationOauthWidget::clear()
{
    while (!mMainLayout->isEmpty()) {
        mMainLayout->removeWidget(mAuthenticationButtonList.first());
    }
    qDeleteAll(mAuthenticationButtonList);
    mAuthenticationButtonList.clear();
}

#include "moc_authenticationoauthwidget.cpp"
