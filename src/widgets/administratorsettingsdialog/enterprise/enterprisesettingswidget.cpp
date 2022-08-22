/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "enterprisesettingswidget.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLineEdit>

EnterpriseSettingsWidget::EnterpriseSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnterpriseLicense(new QLineEdit(this))
{
    mEnterpriseLicense->setObjectName(QStringLiteral("mEnterpriseLicense"));
    mEnterpriseLicense->setToolTip(
        i18n("If your workspace is registered and license is provided by Rocket.Chat Cloud you don't need to manually update the license here."));
    addLineEdit(i18n("Enterprise License"), mEnterpriseLicense, QStringLiteral("Enterprise_License"));

    // TODO add status
}

EnterpriseSettingsWidget::~EnterpriseSettingsWidget() = default;

void EnterpriseSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnterpriseLicense, mapSettings);
}
