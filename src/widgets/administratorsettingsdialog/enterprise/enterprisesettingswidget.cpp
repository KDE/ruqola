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
    addLineEdit(i18n("Enterprise License"), mEnterpriseLicense, QStringLiteral("Enterprise_License"));
}

EnterpriseSettingsWidget::~EnterpriseSettingsWidget() = default;

void EnterpriseSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnterpriseLicense, mapSettings);
}
