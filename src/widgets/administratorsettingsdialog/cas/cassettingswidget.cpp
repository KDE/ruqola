/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "cassettingswidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QCheckBox>
#include <QFormLayout>

CasSettingsWidget::CasSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mEnabled(new QCheckBox(i18n("Enabled"), this))
    , mTrustCASUsername(new QCheckBox(i18n("Trust CAS Username"), this))
{
    mEnabled->setObjectName(QStringLiteral("mEnabled"));
    mMainLayout->addWidget(mEnabled);
    connectCheckBox(mEnabled, QStringLiteral("CAS_enabled"));

    mTrustCASUsername->setObjectName(QStringLiteral("mTrustCASUsername"));
    mMainLayout->addWidget(mTrustCASUsername);
    connectCheckBox(mTrustCASUsername, QStringLiteral("CAS_trust_username"));
}

CasSettingsWidget::~CasSettingsWidget() = default;

void CasSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings, false);
    initializeWidget(mTrustCASUsername, mapSettings, false);
}
