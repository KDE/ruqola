/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ldapsettingswidget.h"

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

LDapSettingsWidget::LDapSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18n("LDAP Enabled"), this))
    , mHost(new QLineEdit(this))
{
    mEnabled->setObjectName(QStringLiteral("mEnabled"));
    mEnabled->setToolTip(i18n("Attempt to utilize LDAP for authentication."));
    mMainLayout->addWidget(mEnabled);
    connectCheckBox(mEnabled, QStringLiteral("LDAP_Enable"));

    mHost->setObjectName(QStringLiteral("mHost"));
    addLineEdit(i18n("Host"), mHost, QStringLiteral("LDAP_Host"));
}

LDapSettingsWidget::~LDapSettingsWidget() = default;

void LDapSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings);
    initializeWidget(mHost, mapSettings);
}
