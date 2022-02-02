/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroautheditwidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>

AdministratorOauthEditWidget::AdministratorOauthEditWidget(QWidget *parent)
    : QWidget{parent}
    , mActiveCheckBox(new QCheckBox(i18n("Active"), this))
    , mApplicationName(new QLineEdit(this))
    , mRedirectUrl(new QLineEdit(this))
    , mClientId(new QLineEdit(this))
    , mClientSecret(new QLineEdit(this))
    , mAuthorizationUrl(new QLineEdit(this))
    , mAccessTokenUrl(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mActiveCheckBox->setObjectName(QStringLiteral("mActiveCheckBox"));
    mainLayout->addWidget(mActiveCheckBox);
    mActiveCheckBox->setChecked(false);

    mApplicationName->setObjectName(QStringLiteral("mApplicationName"));
    mainLayout->addRow(i18n("Name:"), mApplicationName);

    mRedirectUrl->setObjectName(QStringLiteral("mRedirectUrl"));
    mainLayout->addRow(i18n("Redirect Url:"), mRedirectUrl);

    mClientId->setObjectName(QStringLiteral("mClientId"));
    mainLayout->addRow(i18n("Client ID:"), mClientId);

    mClientSecret->setObjectName(QStringLiteral("mClientServer"));
    mainLayout->addRow(i18n("Client Secret:"), mClientSecret);

    mAuthorizationUrl->setObjectName(QStringLiteral("mAuthorizationUrl"));
    mainLayout->addRow(i18n("Authorization Url:"), mAuthorizationUrl);

    mAccessTokenUrl->setObjectName(QStringLiteral("mAccessTokenUrl"));
    mainLayout->addRow(i18n("Access Token URL:"), mAccessTokenUrl);
}

AdministratorOauthEditWidget::~AdministratorOauthEditWidget()
{
}

AdministratorOauthEditWidget::OauthEditInfo AdministratorOauthEditWidget::oauthInfo() const
{
    // TODO
    return {};
}

void AdministratorOauthEditWidget::setOauthInfo(const OauthEditInfo &info)
{
    // TODO
}
