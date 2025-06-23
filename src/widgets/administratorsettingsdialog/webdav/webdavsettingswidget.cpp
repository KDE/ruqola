/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "webdavsettingswidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>

WebDavSettingsWidget::WebDavSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mEnabled(new QCheckBox(i18nc("@option:check", "Webdav Integration Enabled"), this))
{
    mEnabled->setObjectName(u"mEnabled"_s);
    addCheckBox(mEnabled, u"Webdav_Integration_Enabled"_s);
}

WebDavSettingsWidget::~WebDavSettingsWidget() = default;

void WebDavSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mEnabled, mapSettings, false);
}

#include "moc_webdavsettingswidget.cpp"
