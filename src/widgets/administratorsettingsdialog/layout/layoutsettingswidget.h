/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "administratorsettingsdialog/settingswidgetbase.h"
#include "libruqolawidgets_private_export.h"
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT LayoutSettingsWidget : public SettingsWidgetBase
{
    Q_OBJECT
public:
    explicit LayoutSettingsWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~LayoutSettingsWidget() override;

    void initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings) override;

private:
    QCheckBox *const mDisplayRoles;
    QCheckBox *const mUseRealName;
    QCheckBox *const mUseFullNameGenerateDefaultAvatar;
    QCheckBox *const mAllowSpecialCharactersRoomNames;
    QCheckBox *const mMergePrivateGroupsWithChannels;
    QCheckBox *const mGroupChannelsByType;
    QSpinBox *const mNumberUsersAutocompletion;
};
