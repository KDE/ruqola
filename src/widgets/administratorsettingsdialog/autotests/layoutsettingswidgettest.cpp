/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "layoutsettingswidgettest.h"
#include "administratorsettingsdialog/layout/layoutsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QSpinBox>
#include <QTest>
QTEST_MAIN(LayoutSettingsWidgetTest)

LayoutSettingsWidgetTest::LayoutSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void LayoutSettingsWidgetTest::shouldHaveDefaultValues()
{
    LayoutSettingsWidget w(nullptr);
    auto mDisplayRoles = w.findChild<QCheckBox *>(QStringLiteral("mDisplayRoles"));
    QVERIFY(mDisplayRoles);
    QVERIFY(!mDisplayRoles->isChecked());
    QVERIFY(!mDisplayRoles->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisplayRoles), QStringLiteral("UI_DisplayRoles"));

    auto mUseRealName = w.findChild<QCheckBox *>(QStringLiteral("mUseRealName"));
    QVERIFY(mUseRealName);
    QVERIFY(!mUseRealName->isChecked());
    QVERIFY(!mUseRealName->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUseRealName), QStringLiteral("UI_Use_Real_Name"));

    auto mUseFullNameGenerateDefaultAvatar = w.findChild<QCheckBox *>(QStringLiteral("mUseFullNameGenerateDefaultAvatar"));
    QVERIFY(mUseFullNameGenerateDefaultAvatar);
    QVERIFY(!mUseFullNameGenerateDefaultAvatar->isChecked());
    QVERIFY(!mUseFullNameGenerateDefaultAvatar->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUseFullNameGenerateDefaultAvatar), QStringLiteral("UI_Use_Name_Avatar"));

    auto mAllowSpecialCharactersRoomNames = w.findChild<QCheckBox *>(QStringLiteral("mAllowSpecialCharactersRoomNames"));
    QVERIFY(mAllowSpecialCharactersRoomNames);
    QVERIFY(!mAllowSpecialCharactersRoomNames->isChecked());
    QVERIFY(!mAllowSpecialCharactersRoomNames->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowSpecialCharactersRoomNames), QStringLiteral("UI_Allow_room_names_with_special_chars"));

    auto mMergePrivateGroupsWithChannels = w.findChild<QCheckBox *>(QStringLiteral("mMergePrivateGroupsWithChannels"));
    QVERIFY(mMergePrivateGroupsWithChannels);
    QVERIFY(!mMergePrivateGroupsWithChannels->isChecked());
    QVERIFY(!mMergePrivateGroupsWithChannels->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMergePrivateGroupsWithChannels), QStringLiteral("UI_Merge_Channels_Groups"));

    auto mGroupChannelsByType = w.findChild<QCheckBox *>(QStringLiteral("mGroupChannelsByType"));
    QVERIFY(mGroupChannelsByType);
    QVERIFY(!mGroupChannelsByType->isChecked());
    QVERIFY(!mGroupChannelsByType->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGroupChannelsByType), QStringLiteral("UI_Group_Channels_By_Type"));

    auto mNumberUsersAutocompletion = w.findChild<QSpinBox *>(QStringLiteral("mNumberUsersAutocompletion"));
    QVERIFY(mNumberUsersAutocompletion);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mNumberUsersAutocompletion), QStringLiteral("Number_of_users_autocomplete_suggestions"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Number_of_users_autocomplete_suggestions"));
}

#include "moc_layoutsettingswidgettest.cpp"
