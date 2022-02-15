/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "confirmpasswordwidget.h"

#include <KAuthorized>
#include <KLocalizedString>
#include <KPasswordLineEdit>
#include <QLabel>
#include <QVBoxLayout>

ConfirmPasswordWidget::ConfirmPasswordWidget(QWidget *parent)
    : QWidget{parent}
    , mPasswordLineEdit(new KPasswordLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("For your security, you must enter your current password to continue"), this);
    label->setObjectName(QStringLiteral("label"));
    mainLayout->addWidget(label);

    mPasswordLineEdit->setObjectName(QStringLiteral("mPasswordLineEdit"));
    mainLayout->addWidget(mPasswordLineEdit);
    mPasswordLineEdit->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
}

ConfirmPasswordWidget::~ConfirmPasswordWidget() = default;

const QString ConfirmPasswordWidget::password() const
{
    return mPasswordLineEdit->password();
}
