/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "generalsettingswidgettest.h"
#include "administratorsettingsdialog/general/generalsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTest>

QTEST_MAIN(GeneralSettingsWidgetTest)
GeneralSettingsWidgetTest::GeneralSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void GeneralSettingsWidgetTest::shouldHaveDefaultValues()
{
    GeneralSettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mEnableFavoriteRooms = w.findChild<QCheckBox *>(QStringLiteral("mEnableFavoriteRooms"));
    QVERIFY(mEnableFavoriteRooms);
    QVERIFY(!mEnableFavoriteRooms->isChecked());
    QVERIFY(!mEnableFavoriteRooms->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableFavoriteRooms), QStringLiteral("Favorite_Rooms"));

    auto mSiteUrl = w.findChild<QLineEdit *>(QStringLiteral("mSiteUrl"));
    QVERIFY(mSiteUrl);
    QVERIFY(mSiteUrl->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSiteUrl), QStringLiteral("Site_Url"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Site_Url"));

    auto mSiteName = w.findChild<QLineEdit *>(QStringLiteral("mSiteName"));
    QVERIFY(mSiteName);
    QVERIFY(mSiteName->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSiteName), QStringLiteral("Site_Name"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Site_Name"));

    auto utf8Label = w.findChild<QLabel *>(QStringLiteral("utf8Label"));
    QVERIFY(utf8Label);
    QVERIFY(!utf8Label->text().isEmpty());

    auto mUTF8UsernamesValidation = w.findChild<QLineEdit *>(QStringLiteral("mUTF8UsernamesValidation"));
    QVERIFY(mUTF8UsernamesValidation);
    QVERIFY(mUTF8UsernamesValidation->text().isEmpty());
    QVERIFY(!mUTF8UsernamesValidation->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUTF8UsernamesValidation), QStringLiteral("UTF8_User_Names_Validation"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("UTF8_User_Names_Validation"));

    auto mUTF8ChannelNamesValidation = w.findChild<QLineEdit *>(QStringLiteral("mUTF8ChannelNamesValidation"));
    QVERIFY(mUTF8ChannelNamesValidation);
    QVERIFY(mUTF8UsernamesValidation->text().isEmpty());
    QVERIFY(!mUTF8ChannelNamesValidation->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUTF8ChannelNamesValidation), QStringLiteral("UTF8_Channel_Names_Validation"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("UTF8_Channel_Names_Validation"));

    auto mFirstChannelAfterLogin = w.findChild<QLineEdit *>(QStringLiteral("mFirstChannelAfterLogin"));
    QVERIFY(mFirstChannelAfterLogin);
    QVERIFY(mFirstChannelAfterLogin->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFirstChannelAfterLogin), QStringLiteral("First_Channel_After_Login"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("First_Channel_After_Login"));

    auto mDeepLinkUrl = w.findChild<QLineEdit *>(QStringLiteral("mDeepLinkUrl"));
    QVERIFY(mDeepLinkUrl);
    QVERIFY(mDeepLinkUrl->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDeepLinkUrl), QStringLiteral("DeepLink_Url"));

    auto mCDNPrefix = w.findChild<QLineEdit *>(QStringLiteral("mCDNPrefix"));
    QVERIFY(mCDNPrefix);
    QVERIFY(mCDNPrefix->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mCDNPrefix), QStringLiteral("CDN_PREFIX"));

    auto mUnreadCount = w.findChild<QComboBox *>(QStringLiteral("mUnreadCount"));
    QVERIFY(mUnreadCount);
    QCOMPARE(mUnreadCount->count(), 4);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUnreadCount), QStringLiteral("Unread_Count"));

    auto mUnreadCountDirectMessages = w.findChild<QComboBox *>(QStringLiteral("mUnreadCountDirectMessages"));
    QVERIFY(mUnreadCountDirectMessages);
    QCOMPARE(mUnreadCountDirectMessages->count(), 2);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUnreadCountDirectMessages), QStringLiteral("Unread_Count_DM"));

    auto mEnableUpdateChecker = w.findChild<QCheckBox *>(QStringLiteral("mEnableUpdateChecker"));
    QVERIFY(mEnableUpdateChecker);
    QVERIFY(!mEnableUpdateChecker->isChecked());
    QVERIFY(!mEnableUpdateChecker->text().isEmpty());
    QVERIFY(!mEnableUpdateChecker->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableUpdateChecker), QStringLiteral("Update_EnableChecker"));

    auto mUpdateLatestAvailableVersion = w.findChild<QLineEdit *>(QStringLiteral("mUpdateLatestAvailableVersion"));
    QVERIFY(mUpdateLatestAvailableVersion);
    QVERIFY(mUpdateLatestAvailableVersion->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUpdateLatestAvailableVersion), QStringLiteral("Update_LatestAvailableVersion"));

    auto updateLabel = w.findChild<QLabel *>(QStringLiteral("updateLabel"));
    QVERIFY(updateLabel);
    QVERIFY(!updateLabel->text().isEmpty());

    auto timeZoneLabel = w.findChild<QLabel *>(QStringLiteral("timeZoneLabel"));
    QVERIFY(timeZoneLabel);
    QVERIFY(!timeZoneLabel->text().isEmpty());

    auto mDefaultTimeZone = w.findChild<QComboBox *>(QStringLiteral("mDefaultTimeZone"));
    QVERIFY(mDefaultTimeZone);
    QCOMPARE(mDefaultTimeZone->count(), 3);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDefaultTimeZone), QStringLiteral("Default_Timezone_For_Reporting"));

    auto mUTF8NamesSlugify = w.findChild<QCheckBox *>(QStringLiteral("mUTF8NamesSlugify"));
    QVERIFY(mUTF8NamesSlugify);
    QVERIFY(!mUTF8NamesSlugify->isChecked());
    QVERIFY(!mUTF8NamesSlugify->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUTF8NamesSlugify), QStringLiteral("UTF8_Names_Slugify"));

    auto reportingLabel = w.findChild<QLabel *>(QStringLiteral("reportingLabel"));
    QVERIFY(reportingLabel);
    QVERIFY(!reportingLabel->text().isEmpty());

    auto mSendStatisticsRocketChat = w.findChild<QCheckBox *>(QStringLiteral("mSendStatisticsRocketChat"));
    QVERIFY(mSendStatisticsRocketChat);
    QVERIFY(!mSendStatisticsRocketChat->isChecked());
    QVERIFY(!mSendStatisticsRocketChat->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSendStatisticsRocketChat), QStringLiteral("Statistics_reporting"));

    auto notificationLabel = w.findChild<QLabel *>(QStringLiteral("notificationLabel"));
    QVERIFY(notificationLabel);
    QVERIFY(!notificationLabel->text().isEmpty());

    auto mMaxRoomMembersDisablingMessageNotifications = w.findChild<QSpinBox *>(QStringLiteral("mMaxRoomMembersDisablingMessageNotifications"));
    QVERIFY(mMaxRoomMembersDisablingMessageNotifications);
    QVERIFY(!mMaxRoomMembersDisablingMessageNotifications->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaxRoomMembersDisablingMessageNotifications), QStringLiteral("Notifications_Max_Room_Members"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Notifications_Max_Room_Members"));

    auto streamCastLabel = w.findChild<QLabel *>(QStringLiteral("streamCastLabel"));
    QVERIFY(streamCastLabel);
    QVERIFY(!streamCastLabel->text().isEmpty());

    auto mStreamCastAddress = w.findChild<QLineEdit *>(QStringLiteral("mStreamCastAddress"));
    QVERIFY(mStreamCastAddress);
    QVERIFY(mStreamCastAddress->text().isEmpty());
    QVERIFY(!mStreamCastAddress->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mStreamCastAddress), QStringLiteral("Stream_Cast_Address"));
}

#include "moc_generalsettingswidgettest.cpp"
