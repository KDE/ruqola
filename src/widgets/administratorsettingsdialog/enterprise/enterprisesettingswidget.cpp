/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "enterprisesettingswidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

EnterpriseSettingsWidget::EnterpriseSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnterpriseLicense(new QLineEdit(this))
    , mStatus(new QLabel(this))
{
    mEnterpriseLicense->setObjectName(u"mEnterpriseLicense"_s);
    mEnterpriseLicense->setToolTip(
        i18n("If your workspace is registered and license is provided by Rocket.Chat Cloud you don't need to manually update the license here."));
    addLineEdit(i18n("Enterprise License"), mEnterpriseLicense, u"Enterprise_License"_s);

    mStatus->setObjectName(u"mStatus"_s);
    addLabel(i18n("Status"), mStatus, u"Enterprise_License_Status"_s);
}

EnterpriseSettingsWidget::~EnterpriseSettingsWidget() = default;

void EnterpriseSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mEnterpriseLicense, mapSettings, QString());
    initializeWidget(mStatus, mapSettings, QString());
}

#include "moc_enterprisesettingswidget.cpp"
