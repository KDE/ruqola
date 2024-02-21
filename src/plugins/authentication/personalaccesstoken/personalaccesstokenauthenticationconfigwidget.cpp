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
    , mUserLineEdit(new QLineEdit(this))
    , mPersonalAccessTokenLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mUserLineEdit->setObjectName(QStringLiteral("mUserLineEdit"));
    mPersonalAccessTokenLineEdit->setObjectName(QStringLiteral("mUserLineEdit"));

    KLineEditEventHandler::catchReturnKey(mUserLineEdit);
    KLineEditEventHandler::catchReturnKey(mPersonalAccessTokenLineEdit);

    mainLayout->addRow(i18n("User Id:"), mUserLineEdit);
    mainLayout->addRow(i18n("Token:"), mPersonalAccessTokenLineEdit);
    connect(mUserLineEdit, &QLineEdit::textChanged, this, &PersonalAccessTokenAuthenticationConfigWidget::slotEnableOkButton);
}

PersonalAccessTokenAuthenticationConfigWidget::~PersonalAccessTokenAuthenticationConfigWidget() = default;

PersonalAccessTokenPluginUtil::PersonalAccessTokenPluginInfo PersonalAccessTokenAuthenticationConfigWidget::info() const
{
    PersonalAccessTokenPluginUtil::PersonalAccessTokenPluginInfo info;
    info.token = mPersonalAccessTokenLineEdit->text();
    info.userId = mUserLineEdit->text();
    return info;
}

void PersonalAccessTokenAuthenticationConfigWidget::slotEnableOkButton()
{
    Q_EMIT enableOkButton(!mPersonalAccessTokenLineEdit->text().isEmpty() || !mUserLineEdit->text().isEmpty());
}

#include "moc_personalaccesstokenauthenticationconfigwidget.cpp"
