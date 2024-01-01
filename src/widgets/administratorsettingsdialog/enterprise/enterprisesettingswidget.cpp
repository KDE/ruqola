/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "enterprisesettingswidget.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

EnterpriseSettingsWidget::EnterpriseSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnterpriseLicense(new QLineEdit(this))
    , mStatus(new QLabel(this))
{
    mEnterpriseLicense->setObjectName(QStringLiteral("mEnterpriseLicense"));
    mEnterpriseLicense->setToolTip(
        i18n("If your workspace is registered and license is provided by Rocket.Chat Cloud you don't need to manually update the license here."));
    addLineEdit(i18n("Enterprise License"), mEnterpriseLicense, QStringLiteral("Enterprise_License"));

    mStatus->setObjectName(QStringLiteral("mStatus"));
    addLabel(i18n("Status"), mStatus, QStringLiteral("Enterprise_License_Status"));
}

EnterpriseSettingsWidget::~EnterpriseSettingsWidget() = default;

void EnterpriseSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnterpriseLicense, mapSettings, QString());
    initializeWidget(mStatus, mapSettings, QString());
}

#include "moc_enterprisesettingswidget.cpp"
