/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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

    void initialize(const QMap<QString, QVariant> &mapSettings) override;

private:
    QCheckBox *const mEnableE2E;
    QCheckBox *const mEnableEncryptionDirectRoomsByDefault;
    QCheckBox *const mEnableEncryptionPrivateRoomsByDefault;
    QCheckBox *const mEnableOtr;
};
