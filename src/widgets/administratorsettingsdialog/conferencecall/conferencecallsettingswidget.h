/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class RocketChatAccount;
class QComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConferenceCallSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit ConferenceCallSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ConferenceCallSettingsWidget() override;

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QComboBox *const mDefaultProvider;
};
