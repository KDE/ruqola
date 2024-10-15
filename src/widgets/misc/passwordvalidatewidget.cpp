/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordvalidatewidget.h"
#include <KLocalizedString>
#include <QGridLayout>
#include <QLabel>

PasswordValidateWidget::PasswordValidateWidget(QWidget *parent)
    : QWidget{parent}
    , mGridLayout(new QGridLayout(this))
{
    mGridLayout->setObjectName(QStringLiteral("mGridLayout"));
    mGridLayout->setContentsMargins({});
    mGridLayout->setSpacing(0);
}

PasswordValidateWidget::~PasswordValidateWidget() = default;

void PasswordValidateWidget::setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings)
{
    if (mPasswordSettings != passwordSettings) {
        mPasswordSettings = passwordSettings;
        initializeWidget();
    }
}

void PasswordValidateWidget::initializeWidget()
{
    // clear grid layout
    if (!mPasswordSettings.accountsPasswordPolicyEnabled) {
        setVisible(false);
    } else {
        setVisible(true);
        const RuqolaServerConfig::PasswordSettings::PasswordSettingChecks checks = mPasswordSettings.passwordValidChecks();
        // TODO
    }
}

void PasswordValidateWidget::validatePassword(const QString &password)
{
    // TODO
}

PasswordValidateLabel::PasswordValidateLabel(QWidget *parent)
    : QWidget(parent)
{
}

PasswordValidateLabel::~PasswordValidateLabel() = default;

#include "moc_passwordvalidatewidget.cpp"
