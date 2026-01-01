/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "layoutsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/layout/layoutsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QSpinBox>
QTEST_MAIN(LayoutSettingsWidgetTest)

LayoutSettingsWidgetTest::LayoutSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void LayoutSettingsWidgetTest::shouldHaveDefaultValues()
{
    LayoutSettingsWidget w(nullptr);
    auto mDisplayRoles = w.findChild<QCheckBox *>(u"mDisplayRoles"_s);
    QVERIFY(mDisplayRoles);
    QVERIFY(!mDisplayRoles->isChecked());
    QVERIFY(!mDisplayRoles->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDisplayRoles), u"UI_DisplayRoles"_s);

    auto mUseRealName = w.findChild<QCheckBox *>(u"mUseRealName"_s);
    QVERIFY(mUseRealName);
    QVERIFY(!mUseRealName->isChecked());
    QVERIFY(!mUseRealName->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUseRealName), u"UI_Use_Real_Name"_s);

    auto mUseFullNameGenerateDefaultAvatar = w.findChild<QCheckBox *>(u"mUseFullNameGenerateDefaultAvatar"_s);
    QVERIFY(mUseFullNameGenerateDefaultAvatar);
    QVERIFY(!mUseFullNameGenerateDefaultAvatar->isChecked());
    QVERIFY(!mUseFullNameGenerateDefaultAvatar->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUseFullNameGenerateDefaultAvatar), u"UI_Use_Name_Avatar"_s);

    auto mAllowSpecialCharactersRoomNames = w.findChild<QCheckBox *>(u"mAllowSpecialCharactersRoomNames"_s);
    QVERIFY(mAllowSpecialCharactersRoomNames);
    QVERIFY(!mAllowSpecialCharactersRoomNames->isChecked());
    QVERIFY(!mAllowSpecialCharactersRoomNames->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAllowSpecialCharactersRoomNames), u"UI_Allow_room_names_with_special_chars"_s);

    auto mMergePrivateGroupsWithChannels = w.findChild<QCheckBox *>(u"mMergePrivateGroupsWithChannels"_s);
    QVERIFY(mMergePrivateGroupsWithChannels);
    QVERIFY(!mMergePrivateGroupsWithChannels->isChecked());
    QVERIFY(!mMergePrivateGroupsWithChannels->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMergePrivateGroupsWithChannels), u"UI_Merge_Channels_Groups"_s);

    auto mGroupChannelsByType = w.findChild<QCheckBox *>(u"mGroupChannelsByType"_s);
    QVERIFY(mGroupChannelsByType);
    QVERIFY(!mGroupChannelsByType->isChecked());
    QVERIFY(!mGroupChannelsByType->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mGroupChannelsByType), u"UI_Group_Channels_By_Type"_s);

    auto mNumberUsersAutocompletion = w.findChild<QSpinBox *>(u"mNumberUsersAutocompletion"_s);
    QVERIFY(mNumberUsersAutocompletion);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mNumberUsersAutocompletion), u"Number_of_users_autocomplete_suggestions"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Number_of_users_autocomplete_suggestions"_s);
}

#include "moc_layoutsettingswidgettest.cpp"
