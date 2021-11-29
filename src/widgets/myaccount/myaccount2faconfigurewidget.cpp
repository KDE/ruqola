/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2faconfigurewidget.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QVBoxLayout>

MyAccount2FaConfigureWidget::MyAccount2FaConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mActivate2FAViaEmailCheckbox(new QCheckBox(i18n("Activate Two Authentication Factor via Email"), this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mActivate2FAViaEmailCheckbox->setObjectName(QStringLiteral("mActivate2FAViaEmailCheckbox"));
    mainLayout->addWidget(mActivate2FAViaEmailCheckbox);

    mainLayout->addStretch(1);
}

MyAccount2FaConfigureWidget::~MyAccount2FaConfigureWidget() = default;

void MyAccount2FaConfigureWidget::load()
{
    mActivate2FAViaEmailCheckbox->setChecked(mRocketChatAccount->ownUser().servicePassword().email2faEnabled());
}

void MyAccount2FaConfigureWidget::save()
{
    // TODO verify it
    mRocketChatAccount->enable2FaEmailJob(mActivate2FAViaEmailCheckbox->isChecked());
}
