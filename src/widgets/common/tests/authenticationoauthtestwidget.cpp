/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "authenticationoauthtestwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "common/authenticationoauthwidget.h"
#include <QPushButton>
#include <QVBoxLayout>

AuthenticationOauthTestWidget::AuthenticationOauthTestWidget(QWidget *parent)
    : QWidget{parent}
    , mAuthenticationOauthWidget(new AuthenticationOauthWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});

    auto button = new QPushButton(u"Add button"_s, this);
    mainLayout->addWidget(button);

    connect(button, &QPushButton::clicked, this, [this]() {
        static int val = 0;
        AuthenticationInfo info;
        info.setName(u"foo %1"_s.arg(val++));
        mAuthenticationOauthWidget->addAuthenticationMethod(info);
    });

    mainLayout->addWidget(mAuthenticationOauthWidget);
    mAuthenticationOauthWidget->setVisible(true);
}

#include "moc_authenticationoauthtestwidget.cpp"
