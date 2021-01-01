/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "twoauthenticationpasswordwidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <KPasswordLineEdit>

#include <QHBoxLayout>
#include <QPushButton>

TwoAuthenticationPasswordWidget::TwoAuthenticationPasswordWidget(QWidget *parent)
    : QWidget(parent)
{
    auto twoFactorLayout = new QHBoxLayout(this);
    twoFactorLayout->setObjectName(QStringLiteral("twoFactorLayout"));
    twoFactorLayout->setContentsMargins({});

    mTwoFactorAuthenticationPasswordLineEdit = new KPasswordLineEdit(this);
    mTwoFactorAuthenticationPasswordLineEdit->setObjectName(QStringLiteral("mTwoFactorAuthenticationPasswordLineEdit"));
    mTwoFactorAuthenticationPasswordLineEdit->lineEdit()->setPlaceholderText(i18n("Enter code"));
    twoFactorLayout->addWidget(mTwoFactorAuthenticationPasswordLineEdit);

    QPushButton *sendNewEmailCode = new QPushButton(i18n("Send new code"), this);
    sendNewEmailCode->setObjectName(QStringLiteral("sendNewEmailCode"));
    twoFactorLayout->addWidget(sendNewEmailCode);
    connect(sendNewEmailCode, &QPushButton::clicked, this, &TwoAuthenticationPasswordWidget::slotSendNewEmailCode);
}

TwoAuthenticationPasswordWidget::~TwoAuthenticationPasswordWidget()
{
}

QString TwoAuthenticationPasswordWidget::code() const
{
    return mTwoFactorAuthenticationPasswordLineEdit->lineEdit()->text();
}

void TwoAuthenticationPasswordWidget::clear()
{
    mTwoFactorAuthenticationPasswordLineEdit->lineEdit()->clear();
}

void TwoAuthenticationPasswordWidget::slotSendNewEmailCode()
{
    auto *rocketChatAccount = Ruqola::self()->rocketChatAccount();
    rocketChatAccount->sendUserEmailCode();
}
