/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallsettingswidget.h"
#include <KLocalizedString>
#include <QComboBox>
#include <QFormLayout>

ConferenceCallSettingsWidget::ConferenceCallSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mDefaultProvider(new QComboBox(this))
{
    mDefaultProvider->setObjectName(QStringLiteral("mDefaultProvider"));
    // TODO ?
    const QMap<QString, QString> maps = {};
    addComboBox(i18n("Default Provider"), maps, mDefaultProvider, QStringLiteral("VideoConf_Default_Provider"));
}

ConferenceCallSettingsWidget::~ConferenceCallSettingsWidget() = default;

void ConferenceCallSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mDefaultProvider, mapSettings, QString());
}
