/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "personalaccesstokenauthenticationconfigwidget.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

PersonalAccessTokenAuthenticationConfigWidget::PersonalAccessTokenAuthenticationConfigWidget(QWidget *parent)
    : QWidget{parent}
    , mServerNameLineEdit(new QLineEdit(this))
    , mAccountNameLineEdit(new QLineEdit(this))
    , mUserLineEdit(new QLineEdit(this))
    , mPersonalAccessTokenLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mServerNameLineEdit->setObjectName(QStringLiteral("mServerNameLineEdit"));
    mAccountNameLineEdit->setObjectName(QStringLiteral("mAccountNameLineEdit"));

    mUserLineEdit->setObjectName(QStringLiteral("mUserLineEdit"));
    mPersonalAccessTokenLineEdit->setObjectName(QStringLiteral("mPersonalAccessTokenLineEdit"));

    KLineEditEventHandler::catchReturnKey(mUserLineEdit);
    KLineEditEventHandler::catchReturnKey(mPersonalAccessTokenLineEdit);
    KLineEditEventHandler::catchReturnKey(mServerNameLineEdit);
    KLineEditEventHandler::catchReturnKey(mAccountNameLineEdit);

    mainLayout->addRow(i18n("Server URL:"), mServerNameLineEdit);
    mainLayout->addRow(i18n("Account name:"), mAccountNameLineEdit);
    mainLayout->addRow(i18n("User Id:"), mUserLineEdit);
    mainLayout->addRow(i18n("Token:"), mPersonalAccessTokenLineEdit);
    connect(mUserLineEdit, &QLineEdit::textChanged, this, &PersonalAccessTokenAuthenticationConfigWidget::slotEnableOkButton);
    connect(mPersonalAccessTokenLineEdit, &QLineEdit::textChanged, this, &PersonalAccessTokenAuthenticationConfigWidget::slotEnableOkButton);
    connect(mServerNameLineEdit, &QLineEdit::textChanged, this, &PersonalAccessTokenAuthenticationConfigWidget::slotEnableOkButton);
    connect(mAccountNameLineEdit, &QLineEdit::textChanged, this, &PersonalAccessTokenAuthenticationConfigWidget::slotEnableOkButton);
}

PersonalAccessTokenAuthenticationConfigWidget::~PersonalAccessTokenAuthenticationConfigWidget() = default;

PersonalAccessTokenPluginUtil::PersonalAccessTokenPluginInfo PersonalAccessTokenAuthenticationConfigWidget::info() const
{
    PersonalAccessTokenPluginUtil::PersonalAccessTokenPluginInfo info;
    info.token = mPersonalAccessTokenLineEdit->text();
    info.userId = mUserLineEdit->text();
    info.accountName = mAccountNameLineEdit->text();
    info.serverUrl = mServerNameLineEdit->text();
    return info;
}

void PersonalAccessTokenAuthenticationConfigWidget::setInfo(const PersonalAccessTokenPluginUtil::PersonalAccessTokenPluginInfo &info)
{
    mPersonalAccessTokenLineEdit->setText(info.token);
    mUserLineEdit->setText(info.userId);
    mAccountNameLineEdit->setText(info.accountName);
    mServerNameLineEdit->setText(info.serverUrl);
}

void PersonalAccessTokenAuthenticationConfigWidget::slotEnableOkButton()
{
    Q_EMIT enableOkButton(info().isValid());
}

#include "moc_personalaccesstokenauthenticationconfigwidget.cpp"
