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

#include "passwordconfirmwidget.h"

#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QFormLayout>

PasswordConfirmWidget::PasswordConfirmWidget(QWidget *parent)
    : QWidget(parent)
    , mNewPasswordLineEdit(new KPasswordLineEdit(this))
    , mConfirmPasswordLineEdit(new KPasswordLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mNewPasswordLineEdit->setObjectName(QStringLiteral("mNewPasswordLineEdit"));
    mainLayout->addRow(i18n("New Password:"), mNewPasswordLineEdit);
    mNewPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));

    mConfirmPasswordLineEdit->setObjectName(QStringLiteral("mConfirmPasswordLineEdit"));
    mainLayout->addRow(i18n("Confirm Password:"), mConfirmPasswordLineEdit);
    mConfirmPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
}

PasswordConfirmWidget::~PasswordConfirmWidget() = default;

bool PasswordConfirmWidget::isNewPasswordConfirmed() const
{
    const QString password = mConfirmPasswordLineEdit->password();
    return !password.isEmpty() && (password == mNewPasswordLineEdit->password());
}

QString PasswordConfirmWidget::password() const
{
    return mConfirmPasswordLineEdit->password();
}
