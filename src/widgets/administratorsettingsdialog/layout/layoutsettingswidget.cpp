/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "layoutsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QSpinBox>

LayoutSettingsWidget::LayoutSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mDisplayRoles(new QCheckBox(i18nc("@option:check", "Display Roles"), this))
    , mUseRealName(new QCheckBox(i18nc("@option:check", "Use Real Name"), this))
    , mUseFullNameGenerateDefaultAvatar(new QCheckBox(i18nc("@option:check", "Use Full Name Initials to Generate Default Avatar"), this))
    , mAllowSpecialCharactersRoomNames(new QCheckBox(i18nc("@option:check", "Allow Special Characters in Room Names"), this))
    , mMergePrivateGroupsWithChannels(new QCheckBox(i18nc("@option:check", "Merge Private Groups with Channels"), this))
    , mGroupChannelsByType(new QCheckBox(i18nc("@option:check", "Group channels by type"), this))
    , mNumberUsersAutocompletion(new QSpinBox(this))
{
    mDisplayRoles->setObjectName(QStringLiteral("mDisplayRoles"));
    addCheckBox(mDisplayRoles, QStringLiteral("UI_DisplayRoles"));

    mUseRealName->setObjectName(QStringLiteral("mUseRealName"));
    addCheckBox(mUseRealName, QStringLiteral("UI_Use_Real_Name"));

    mUseFullNameGenerateDefaultAvatar->setObjectName(QStringLiteral("mUseFullNameGenerateDefaultAvatar"));
    addCheckBox(mUseFullNameGenerateDefaultAvatar, QStringLiteral("UI_Use_Name_Avatar"));

    mAllowSpecialCharactersRoomNames->setObjectName(QStringLiteral("mAllowSpecialCharactersRoomNames"));
    addCheckBox(mAllowSpecialCharactersRoomNames, QStringLiteral("UI_Allow_room_names_with_special_chars"));

    mMergePrivateGroupsWithChannels->setObjectName(QStringLiteral("mMergePrivateGroupsWithChannels"));
    mMainLayout->addWidget(mMergePrivateGroupsWithChannels);
    addCheckBox(mMergePrivateGroupsWithChannels, QStringLiteral("UI_Merge_Channels_Groups"));

    mGroupChannelsByType->setObjectName(QStringLiteral("mGroupChannelsByType"));
    addCheckBox(mGroupChannelsByType, QStringLiteral("UI_Group_Channels_By_Type"));

    mNumberUsersAutocompletion->setObjectName(QStringLiteral("mNumberUsersAutocompletion"));
    mNumberUsersAutocompletion->setMaximum(99);
    addSpinbox(i18n("Number of users' autocomplete suggestions"), mNumberUsersAutocompletion, QStringLiteral("Number_of_users_autocomplete_suggestions"));
}

LayoutSettingsWidget::~LayoutSettingsWidget() = default;

void LayoutSettingsWidget::initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    initializeWidget(mDisplayRoles, mapSettings, true);
    initializeWidget(mUseRealName, mapSettings, false);
    initializeWidget(mUseFullNameGenerateDefaultAvatar, mapSettings, false);
    initializeWidget(mAllowSpecialCharactersRoomNames, mapSettings, false);
    initializeWidget(mMergePrivateGroupsWithChannels, mapSettings, true);
    initializeWidget(mGroupChannelsByType, mapSettings, true);
    initializeWidget(mNumberUsersAutocompletion, mapSettings, 5);
}

#include "moc_layoutsettingswidget.cpp"
