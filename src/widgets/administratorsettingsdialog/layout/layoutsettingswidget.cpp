/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "layoutsettingswidget.h"
using namespace Qt::Literals::StringLiterals;

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
    mDisplayRoles->setObjectName(u"mDisplayRoles"_s);
    addCheckBox(mDisplayRoles, u"UI_DisplayRoles"_s);

    mUseRealName->setObjectName(u"mUseRealName"_s);
    addCheckBox(mUseRealName, u"UI_Use_Real_Name"_s);

    mUseFullNameGenerateDefaultAvatar->setObjectName(u"mUseFullNameGenerateDefaultAvatar"_s);
    addCheckBox(mUseFullNameGenerateDefaultAvatar, u"UI_Use_Name_Avatar"_s);

    mAllowSpecialCharactersRoomNames->setObjectName(u"mAllowSpecialCharactersRoomNames"_s);
    addCheckBox(mAllowSpecialCharactersRoomNames, u"UI_Allow_room_names_with_special_chars"_s);

    mMergePrivateGroupsWithChannels->setObjectName(u"mMergePrivateGroupsWithChannels"_s);
    mMainLayout->addWidget(mMergePrivateGroupsWithChannels);
    addCheckBox(mMergePrivateGroupsWithChannels, u"UI_Merge_Channels_Groups"_s);

    mGroupChannelsByType->setObjectName(u"mGroupChannelsByType"_s);
    addCheckBox(mGroupChannelsByType, u"UI_Group_Channels_By_Type"_s);

    mNumberUsersAutocompletion->setObjectName(u"mNumberUsersAutocompletion"_s);
    mNumberUsersAutocompletion->setMaximum(99);
    addSpinbox(i18n("Number of users' autocomplete suggestions"), mNumberUsersAutocompletion, u"Number_of_users_autocomplete_suggestions"_s);
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
