/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "passwordvalidatewidget.h"
#include <KLocalizedString>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
using namespace Qt::Literals::StringLiterals;

PasswordValidateWidget::PasswordValidateWidget(QWidget *parent)
    : QWidget{parent}
    , mGridLayout(new QGridLayout(this))
{
    mGridLayout->setObjectName("mGridLayout"_L1);
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
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName("mainLayout"_L1);
}

PasswordValidateLabel::~PasswordValidateLabel() = default;

#include "moc_passwordvalidatewidget.cpp"
