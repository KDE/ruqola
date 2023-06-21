/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroautheditwidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

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
    mClientId->setReadOnly(true);

    mClientSecret->setObjectName(QStringLiteral("mClientSecret"));
    mainLayout->addRow(i18n("Client Secret:"), mClientSecret);
    mClientSecret->setReadOnly(true);

    mAuthorizationUrl->setObjectName(QStringLiteral("mAuthorizationUrl"));
    mainLayout->addRow(i18n("Authorization Url:"), mAuthorizationUrl);
    mAuthorizationUrl->setReadOnly(true);

    mAccessTokenUrl->setObjectName(QStringLiteral("mAccessTokenUrl"));
    mainLayout->addRow(i18n("Access Token URL:"), mAccessTokenUrl);
    mAccessTokenUrl->setReadOnly(true);

    connect(mApplicationName, &QLineEdit::textEdited, this, &AdministratorOauthEditWidget::slotTextChanged);
    connect(mRedirectUrl, &QLineEdit::textEdited, this, &AdministratorOauthEditWidget::slotTextChanged);
}

AdministratorOauthEditWidget::~AdministratorOauthEditWidget() = default;

void AdministratorOauthEditWidget::slotTextChanged()
{
    Q_EMIT enableOkButton(!mRedirectUrl->text().trimmed().isEmpty() && !mApplicationName->text().trimmed().isEmpty() && !mClientId->text().trimmed().isEmpty()
                          && !mClientSecret->text().trimmed().isEmpty() && !mAuthorizationUrl->text().trimmed().isEmpty()
                          && !mAccessTokenUrl->text().trimmed().isEmpty());
}

AdministratorOauthEditWidget::OauthEditInfo AdministratorOauthEditWidget::oauthInfo() const
{
    AdministratorOauthEditWidget::OauthEditInfo info;
    info.active = mActiveCheckBox->isChecked();
    info.applicationName = mApplicationName->text().trimmed();
    info.redirectUrl = mRedirectUrl->text().trimmed();
    info.clientId = mClientId->text().trimmed();
    info.clientSecret = mClientSecret->text().trimmed();
    info.authorizationUrl = mAuthorizationUrl->text().trimmed();
    info.accessTokenUrl = mAccessTokenUrl->text().trimmed();
    return info;
}

void AdministratorOauthEditWidget::setOauthInfo(const OauthEditInfo &info)
{
    mActiveCheckBox->setChecked(info.active);
    mApplicationName->setText(info.applicationName);
    mRedirectUrl->setText(info.redirectUrl);
    mClientId->setText(info.clientId);
    mClientSecret->setText(info.clientSecret);
    mAuthorizationUrl->setText(info.authorizationUrl);
    mAccessTokenUrl->setText(info.accessTokenUrl);
}

QDebug operator<<(QDebug d, const AdministratorOauthEditWidget::OauthEditInfo &info)
{
    d << "active : " << info.active;
    d << "applicationName : " << info.applicationName;
    d << "redirectUrl : " << info.redirectUrl;
    d << "clientId : " << info.clientId;
    d << "clientSecret : " << info.clientSecret;
    d << "authorizationUrl : " << info.authorizationUrl;
    d << "accessTokenUrl : " << info.accessTokenUrl;
    return d;
}

bool AdministratorOauthEditWidget::OauthEditInfo::isValid() const
{
    return !applicationName.trimmed().isEmpty() && !redirectUrl.trimmed().isEmpty() && !clientId.trimmed().isEmpty() && !clientSecret.trimmed().isEmpty()
        && !authorizationUrl.trimmed().isEmpty() && !accessTokenUrl.trimmed().isEmpty();
}

#include "moc_administratoroautheditwidget.cpp"
