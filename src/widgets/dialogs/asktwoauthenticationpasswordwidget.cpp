/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "asktwoauthenticationpasswordwidget.h"
#include "misc/twoauthenticationpasswordwidget.h"
#include <QVBoxLayout>

AskTwoAuthenticationPasswordWidget::AskTwoAuthenticationPasswordWidget(QWidget *parent)
    : QWidget(parent)
    , mTwoFactorAuthenticationPasswordLineEdit(new TwoAuthenticationPasswordWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});
    mTwoFactorAuthenticationPasswordLineEdit->setObjectName(QStringLiteral("mTwoFactorAuthenticationPasswordLineEdit"));
    mainLayout->addWidget(mTwoFactorAuthenticationPasswordLineEdit);
    connect(mTwoFactorAuthenticationPasswordLineEdit,
            &TwoAuthenticationPasswordWidget::updateButtonOk,
            this,
            &AskTwoAuthenticationPasswordWidget::updateButtonOk);
}

AskTwoAuthenticationPasswordWidget::~AskTwoAuthenticationPasswordWidget() = default;

QString AskTwoAuthenticationPasswordWidget::code() const
{
    return mTwoFactorAuthenticationPasswordLineEdit->code();
}
