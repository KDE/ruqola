/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "layoutsettingswidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QSpinBox>

LayoutSettingsWidget::LayoutSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase(account, parent)
    , mDisplayRoles(new QCheckBox(i18n("Display Roles"), this))
    , mUseRealName(new QCheckBox(i18n("Use Real Name"), this))
    , mUseFullNameGenerateDefaultAvatar(new QCheckBox(i18n("Use Full Name Initials to Generate Default Avatar"), this))
    , mAllowSpecialCharactersRoomNames(new QCheckBox(i18n("Allow Special Characters in Room Names"), this))
    , mMergePrivateGroupsWithChannels(new QCheckBox(i18n("Merge Private Groups with Channels"), this))
    , mGroupChannelsByType(new QCheckBox(i18n("Group channels by type"), this))
    , mNumberUsersAutocompletion(new QSpinBox(this))
{
    mDisplayRoles->setObjectName(QStringLiteral("mDisplayRoles"));
    mMainLayout->addWidget(mDisplayRoles);
    connectCheckBox(mDisplayRoles, QStringLiteral("UI_DisplayRoles"));

    mUseRealName->setObjectName(QStringLiteral("mUseRealName"));
    mMainLayout->addWidget(mUseRealName);
    connectCheckBox(mUseRealName, QStringLiteral("UI_Use_Real_Name"));

    mUseFullNameGenerateDefaultAvatar->setObjectName(QStringLiteral("mUseFullNameGenerateDefaultAvatar"));
    mMainLayout->addWidget(mUseFullNameGenerateDefaultAvatar);
    connectCheckBox(mUseFullNameGenerateDefaultAvatar, QStringLiteral("UI_Use_Name_Avatar"));

    mAllowSpecialCharactersRoomNames->setObjectName(QStringLiteral("mAllowSpecialCharactersRoomNames"));
    mMainLayout->addWidget(mAllowSpecialCharactersRoomNames);
    connectCheckBox(mAllowSpecialCharactersRoomNames, QStringLiteral("UI_Allow_room_names_with_special_chars"));

    mMergePrivateGroupsWithChannels->setObjectName(QStringLiteral("mMergePrivateGroupsWithChannels"));
    mMainLayout->addWidget(mMergePrivateGroupsWithChannels);
    connectCheckBox(mMergePrivateGroupsWithChannels, QStringLiteral("UI_Merge_Channels_Groups"));

    mGroupChannelsByType->setObjectName(QStringLiteral("mGroupChannelsByType"));
    mMainLayout->addWidget(mGroupChannelsByType);
    connectCheckBox(mGroupChannelsByType, QStringLiteral("UI_Group_Channels_By_Type"));

    mNumberUsersAutocompletion->setObjectName(QStringLiteral("mNumberUsersAutocompletion"));
    mNumberUsersAutocompletion->setObjectName(QStringLiteral("mTimeouts"));
    mNumberUsersAutocompletion->setMaximum(99);
    addSpinbox(i18n("Number of users' autocomplete suggestions"), mNumberUsersAutocompletion, QStringLiteral("Number_of_users_autocomplete_suggestions"));
}

LayoutSettingsWidget::~LayoutSettingsWidget() = default;

void LayoutSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    initializeWidget(mDisplayRoles, mapSettings, true);
    initializeWidget(mUseRealName, mapSettings, false);
    initializeWidget(mUseFullNameGenerateDefaultAvatar, mapSettings, false);
    initializeWidget(mAllowSpecialCharactersRoomNames, mapSettings, false);
    initializeWidget(mMergePrivateGroupsWithChannels, mapSettings, true);
    initializeWidget(mGroupChannelsByType, mapSettings, true);
    initializeWidget(mNumberUsersAutocompletion, mapSettings, 5);
}
