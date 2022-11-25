/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

OauthSettingsWidget::OauthSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mSignWithApple(new QCheckBox(i18n("Sign in with Apple"), this))
    , mAccountsOAuthAppleId(new QLineEdit(this))
{
    auto appleLabel = createBoldLabel(i18n("Apple"));
    appleLabel->setObjectName(QStringLiteral("appleLabel"));
    mMainLayout->addWidget(appleLabel);

    mSignWithApple->setObjectName(QStringLiteral("mSignWithApple"));
    mMainLayout->addWidget(mSignWithApple);
    mSignWithApple->setToolTip(i18n("If you want Apple login enabled only on mobile, you can leave all fields empty."));
    connectCheckBox(mSignWithApple, QStringLiteral("Accounts_OAuth_Apple"));

    mAccountsOAuthAppleId->setObjectName(QStringLiteral("mAccountsOAuthAppleId"));
    addLineEdit(i18n("OAuth Apple Id"), mAccountsOAuthAppleId, QStringLiteral("Accounts_OAuth_Apple_id"));
}

OauthSettingsWidget::~OauthSettingsWidget() = default;

void OauthSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mSignWithApple, mapSettings, false);
    initializeWidget(mAccountsOAuthAppleId, mapSettings, QString());
}
