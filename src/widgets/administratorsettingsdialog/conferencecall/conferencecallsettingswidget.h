/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class RocketChatAccount;
class QComboBox;
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConferenceCallSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit ConferenceCallSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ConferenceCallSettingsWidget() override;

    void initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings) override;

private:
    QComboBox *const mDefaultProvider;
    QCheckBox *const mEnableDirectMessage;
    QCheckBox *const mEnablePublicChannels;
    QCheckBox *const mEnablePrivateChannels;
    QCheckBox *const mEnableTeams;
};
