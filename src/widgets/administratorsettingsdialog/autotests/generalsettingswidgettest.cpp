/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "generalsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/general/generalsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

QTEST_MAIN(GeneralSettingsWidgetTest)
GeneralSettingsWidgetTest::GeneralSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void GeneralSettingsWidgetTest::shouldHaveDefaultValues()
{
    GeneralSettingsWidget w(nullptr);
    QVERIFY(w.widgetResizable());

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mEnableFavoriteRooms = w.findChild<QCheckBox *>(u"mEnableFavoriteRooms"_s);
    QVERIFY(mEnableFavoriteRooms);
    QVERIFY(!mEnableFavoriteRooms->isChecked());
    QVERIFY(!mEnableFavoriteRooms->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableFavoriteRooms), u"Favorite_Rooms"_s);

    auto mSiteUrl = w.findChild<QLineEdit *>(u"mSiteUrl"_s);
    QVERIFY(mSiteUrl);
    QVERIFY(mSiteUrl->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSiteUrl), u"Site_Url"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Site_Url"_s);

    auto mSiteName = w.findChild<QLineEdit *>(u"mSiteName"_s);
    QVERIFY(mSiteName);
    QVERIFY(mSiteName->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSiteName), u"Site_Name"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Site_Name"_s);

    auto utf8Label = w.findChild<QLabel *>(u"utf8Label"_s);
    QVERIFY(utf8Label);
    QVERIFY(!utf8Label->text().isEmpty());

    auto mUTF8UsernamesValidation = w.findChild<QLineEdit *>(u"mUTF8UsernamesValidation"_s);
    QVERIFY(mUTF8UsernamesValidation);
    QVERIFY(mUTF8UsernamesValidation->text().isEmpty());
    QVERIFY(!mUTF8UsernamesValidation->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUTF8UsernamesValidation), u"UTF8_User_Names_Validation"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"UTF8_User_Names_Validation"_s);

    auto mUTF8ChannelNamesValidation = w.findChild<QLineEdit *>(u"mUTF8ChannelNamesValidation"_s);
    QVERIFY(mUTF8ChannelNamesValidation);
    QVERIFY(mUTF8UsernamesValidation->text().isEmpty());
    QVERIFY(!mUTF8ChannelNamesValidation->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUTF8ChannelNamesValidation), u"UTF8_Channel_Names_Validation"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"UTF8_Channel_Names_Validation"_s);

    auto mFirstChannelAfterLogin = w.findChild<QLineEdit *>(u"mFirstChannelAfterLogin"_s);
    QVERIFY(mFirstChannelAfterLogin);
    QVERIFY(mFirstChannelAfterLogin->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mFirstChannelAfterLogin), u"First_Channel_After_Login"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"First_Channel_After_Login"_s);

    auto mDeepLinkUrl = w.findChild<QLineEdit *>(u"mDeepLinkUrl"_s);
    QVERIFY(mDeepLinkUrl);
    QVERIFY(mDeepLinkUrl->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDeepLinkUrl), u"DeepLink_Url"_s);

    auto mCDNPrefix = w.findChild<QLineEdit *>(u"mCDNPrefix"_s);
    QVERIFY(mCDNPrefix);
    QVERIFY(mCDNPrefix->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mCDNPrefix), u"CDN_PREFIX"_s);

    auto mUnreadCount = w.findChild<QComboBox *>(u"mUnreadCount"_s);
    QVERIFY(mUnreadCount);
    QCOMPARE(mUnreadCount->count(), 4);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUnreadCount), u"Unread_Count"_s);

    auto mUnreadCountDirectMessages = w.findChild<QComboBox *>(u"mUnreadCountDirectMessages"_s);
    QVERIFY(mUnreadCountDirectMessages);
    QCOMPARE(mUnreadCountDirectMessages->count(), 2);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUnreadCountDirectMessages), u"Unread_Count_DM"_s);

    auto mEnableUpdateChecker = w.findChild<QCheckBox *>(u"mEnableUpdateChecker"_s);
    QVERIFY(mEnableUpdateChecker);
    QVERIFY(!mEnableUpdateChecker->isChecked());
    QVERIFY(!mEnableUpdateChecker->text().isEmpty());
    QVERIFY(!mEnableUpdateChecker->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mEnableUpdateChecker), u"Update_EnableChecker"_s);

    auto mUpdateLatestAvailableVersion = w.findChild<QLineEdit *>(u"mUpdateLatestAvailableVersion"_s);
    QVERIFY(mUpdateLatestAvailableVersion);
    QVERIFY(mUpdateLatestAvailableVersion->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUpdateLatestAvailableVersion), u"Update_LatestAvailableVersion"_s);

    auto updateLabel = w.findChild<QLabel *>(u"updateLabel"_s);
    QVERIFY(updateLabel);
    QVERIFY(!updateLabel->text().isEmpty());

    auto timeZoneLabel = w.findChild<QLabel *>(u"timeZoneLabel"_s);
    QVERIFY(timeZoneLabel);
    QVERIFY(!timeZoneLabel->text().isEmpty());

    auto mDefaultTimeZone = w.findChild<QComboBox *>(u"mDefaultTimeZone"_s);
    QVERIFY(mDefaultTimeZone);
    QCOMPARE(mDefaultTimeZone->count(), 3);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mDefaultTimeZone), u"Default_Timezone_For_Reporting"_s);

    auto mUTF8NamesSlugify = w.findChild<QCheckBox *>(u"mUTF8NamesSlugify"_s);
    QVERIFY(mUTF8NamesSlugify);
    QVERIFY(!mUTF8NamesSlugify->isChecked());
    QVERIFY(!mUTF8NamesSlugify->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUTF8NamesSlugify), u"UTF8_Names_Slugify"_s);

    auto reportingLabel = w.findChild<QLabel *>(u"reportingLabel"_s);
    QVERIFY(reportingLabel);
    QVERIFY(!reportingLabel->text().isEmpty());

    auto mSendStatisticsRocketChat = w.findChild<QCheckBox *>(u"mSendStatisticsRocketChat"_s);
    QVERIFY(mSendStatisticsRocketChat);
    QVERIFY(!mSendStatisticsRocketChat->isChecked());
    QVERIFY(!mSendStatisticsRocketChat->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSendStatisticsRocketChat), u"Statistics_reporting"_s);

    auto notificationLabel = w.findChild<QLabel *>(u"notificationLabel"_s);
    QVERIFY(notificationLabel);
    QVERIFY(!notificationLabel->text().isEmpty());

    auto mMaxRoomMembersDisablingMessageNotifications = w.findChild<QSpinBox *>(u"mMaxRoomMembersDisablingMessageNotifications"_s);
    QVERIFY(mMaxRoomMembersDisablingMessageNotifications);
    QVERIFY(!mMaxRoomMembersDisablingMessageNotifications->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mMaxRoomMembersDisablingMessageNotifications), u"Notifications_Max_Room_Members"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"Notifications_Max_Room_Members"_s);

    auto streamCastLabel = w.findChild<QLabel *>(u"streamCastLabel"_s);
    QVERIFY(streamCastLabel);
    QVERIFY(!streamCastLabel->text().isEmpty());

    auto mStreamCastAddress = w.findChild<QLineEdit *>(u"mStreamCastAddress"_s);
    QVERIFY(mStreamCastAddress);
    QVERIFY(mStreamCastAddress->text().isEmpty());
    QVERIFY(!mStreamCastAddress->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mStreamCastAddress), u"Stream_Cast_Address"_s);

    auto restApiLabel = w.findChild<QLabel *>(u"restApiLabel"_s);
    QVERIFY(restApiLabel);
    QVERIFY(!restApiLabel->text().isEmpty());

    auto mRestApiUpperCountLimit = w.findChild<QSpinBox *>(u"mRestApiUpperCountLimit"_s);
    QVERIFY(mRestApiUpperCountLimit);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mRestApiUpperCountLimit), u"API_Upper_Count_Limit"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"API_Upper_Count_Limit"_s);

#if 0 // RC 8.0.0 we don't have it
    auto mUseRestForDDPCalls = w.findChild<QCheckBox *>(u"mUseRestForDDPCalls"_s);
    QVERIFY(mUseRestForDDPCalls);
    QVERIFY(!mUseRestForDDPCalls->isChecked());
    QVERIFY(!mUseRestForDDPCalls->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mUseRestForDDPCalls), u"API_Use_REST_For_DDP_Calls"_s);
#endif
    auto mApiUserLimit = w.findChild<QSpinBox *>(u"mApiUserLimit"_s);
    QVERIFY(mApiUserLimit);
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mApiUserLimit), u"API_User_Limit"_s);
    SettingsWidgetHelper::checkLabelToolButton(&w, u"API_User_Limit"_s);
}

#include "moc_generalsettingswidgettest.cpp"
