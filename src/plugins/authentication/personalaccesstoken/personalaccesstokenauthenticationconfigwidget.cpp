/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenauthenticationconfigwidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

PersonalAccessTokenAuthenticationConfigWidget::PersonalAccessTokenAuthenticationConfigWidget(QWidget *parent)
    : PluginAuthenticationConfigureWidget{parent}
    , mServerNameLineEdit(new QLineEdit(this))
    , mAccountNameLineEdit(new QLineEdit(this))
    , mUserLineEdit(new QLineEdit(this))
    , mPersonalAccessTokenLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mServerNameLineEdit->setObjectName(u"mServerNameLineEdit"_s);
    mAccountNameLineEdit->setObjectName(u"mAccountNameLineEdit"_s);
    mAccountNameLineEdit->setClearButtonEnabled(true);
    mUserLineEdit->setClearButtonEnabled(true);
    mPersonalAccessTokenLineEdit->setClearButtonEnabled(true);

    mUserLineEdit->setObjectName(u"mUserLineEdit"_s);
    mPersonalAccessTokenLineEdit->setObjectName(u"mPersonalAccessTokenLineEdit"_s);

    KLineEditEventHandler::catchReturnKey(mUserLineEdit);
    KLineEditEventHandler::catchReturnKey(mPersonalAccessTokenLineEdit);
    KLineEditEventHandler::catchReturnKey(mServerNameLineEdit);
    KLineEditEventHandler::catchReturnKey(mAccountNameLineEdit);

    mainLayout->addRow(i18n("Server URL:"), mServerNameLineEdit);
    mainLayout->addRow(i18n("Account name:"), mAccountNameLineEdit);
    mainLayout->addRow(i18n("User Id:"), mUserLineEdit);
    mainLayout->addRow(i18n("Token:"), mPersonalAccessTokenLineEdit);
    connect(mUserLineEdit, &QLineEdit::textChanged, this, &PersonalAccessTokenAuthenticationConfigWidget::slotLoginSettingsChanged);
    connect(mPersonalAccessTokenLineEdit, &QLineEdit::textChanged, this, &PersonalAccessTokenAuthenticationConfigWidget::slotLoginSettingsChanged);
    connect(mServerNameLineEdit, &QLineEdit::textChanged, this, &PersonalAccessTokenAuthenticationConfigWidget::slotLoginSettingsChanged);
    connect(mAccountNameLineEdit, &QLineEdit::textChanged, this, &PersonalAccessTokenAuthenticationConfigWidget::slotLoginSettingsChanged);
}

PersonalAccessTokenAuthenticationConfigWidget::~PersonalAccessTokenAuthenticationConfigWidget() = default;

AccountManager::AccountManagerInfo PersonalAccessTokenAuthenticationConfigWidget::accountInfo()
{
    AccountManager::AccountManagerInfo info;
    info.accountName = mAccountNameLineEdit->text();
    info.serverUrl = mServerNameLineEdit->text();
    info.token = mPersonalAccessTokenLineEdit->text();
    info.userId = mUserLineEdit->text().toLatin1();
    info.authMethodType = AuthenticationManager::AuthMethodType::PersonalAccessToken;
    return info;
}

void PersonalAccessTokenAuthenticationConfigWidget::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    // We can't change mServerNameLineEdit when we edit account
    mServerNameLineEdit->setReadOnly(true);
    mPersonalAccessTokenLineEdit->setText(info.token);
    mUserLineEdit->setText(QString::fromLatin1(info.userId));
    mAccountNameLineEdit->setText(info.accountName);
    mServerNameLineEdit->setText(info.serverUrl);
}

void PersonalAccessTokenAuthenticationConfigWidget::changeAuthenticationWidgetStatus(bool enabled)
{
    // TODO ?
}

void PersonalAccessTokenAuthenticationConfigWidget::slotLoginSettingsChanged()
{
    const QString accountName = mAccountNameLineEdit->text().trimmed();
    Q_EMIT settingsIsValid(!accountName.isEmpty() && !mExistingAccountNames.contains(accountName) && !mServerNameLineEdit->text().trimmed().isEmpty()
                           && !mUserLineEdit->text().trimmed().isEmpty() && !mPersonalAccessTokenLineEdit->text().trimmed().isEmpty());
}

#include "moc_personalaccesstokenauthenticationconfigwidget.cpp"
