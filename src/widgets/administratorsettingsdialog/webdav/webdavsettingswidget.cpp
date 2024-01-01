/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "webdavsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

WebDavSettingsWidget::WebDavSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18n("Webdav Integration Enabled"), this))
{
    mEnabled->setObjectName(QStringLiteral("mEnabled"));
    mMainLayout->addWidget(mEnabled);
    connectCheckBox(mEnabled, QStringLiteral("Webdav_Integration_Enabled"));
}

WebDavSettingsWidget::~WebDavSettingsWidget() = default;

void WebDavSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings, false);
}

#include "moc_webdavsettingswidget.cpp"
