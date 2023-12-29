/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "authenticationoauthtestwidget.h"
#include "common/authenticationoauthwidget.h"
#include <QPushButton>
#include <QVBoxLayout>

AuthenticationOauthTestWidget::AuthenticationOauthTestWidget(QWidget *parent)
    : QWidget{parent}
    , mAuthenticationOauthWidget(new AuthenticationOauthWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});

    auto button = new QPushButton(QStringLiteral("Add button"), this);
    mainLayout->addWidget(button);

    connect(button, &QPushButton::clicked, this, [this]() {
        static int val = 0;
        AuthenticationInfo info;
        info.setName(QStringLiteral("foo %1").arg(val));
        mAuthenticationOauthWidget->addAuthenticationMethod(info);
    });

    mainLayout->addWidget(mAuthenticationOauthWidget);
}
