/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT EncryptionSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit EncryptionSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~EncryptionSettingsWidget() override;

    void initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings) override;

private:
    QCheckBox *const mEnableE2E;
    QCheckBox *const mEnableEncryptionDirectRoomsByDefault;
    QCheckBox *const mEnableEncryptionPrivateRoomsByDefault;
    QCheckBox *const mEnableEncryptFiles;
    QCheckBox *const mAllowUnencryptedMessages;
    QCheckBox *const mEnableMentions;
    QCheckBox *const mEnableOtr;
};
