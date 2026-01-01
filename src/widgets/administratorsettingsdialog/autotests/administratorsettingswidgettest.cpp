/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingswidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorsettingsdialog/accounts/accountsettingswidget.h"
#include "administratorsettingsdialog/administratorsettingswidget.h"
#include "administratorsettingsdialog/analytics/analyticswidget.h"
#include "administratorsettingsdialog/cas/cassettingswidget.h"
#include "administratorsettingsdialog/conferencecall/conferencecallsettingswidget.h"
#include "administratorsettingsdialog/email/emailsettingswidget.h"
#include "administratorsettingsdialog/encryption/encryptionsettingswidget.h"
#include "administratorsettingsdialog/enterprise/enterprisesettingswidget.h"
#include "administratorsettingsdialog/fileupload/fileuploadsettingswidget.h"
#include "administratorsettingsdialog/general/generalsettingswidget.h"
#include "administratorsettingsdialog/irc/ircwidget.h"
#include "administratorsettingsdialog/ircfederation/ircfederationwidget.h"
#include "administratorsettingsdialog/layout/layoutsettingswidget.h"
#include "administratorsettingsdialog/ldap/ldapsettingswidget.h"
#include "administratorsettingsdialog/logs/logssettingswidget.h"
#include "administratorsettingsdialog/message/messagesettingswidget.h"
#include "administratorsettingsdialog/mobile/mobilesettingswidget.h"
#include "administratorsettingsdialog/oauth/oauthsettingswidget.h"
#include "administratorsettingsdialog/password/passwordsettingswidget.h"
#include "administratorsettingsdialog/ratelimiter/ratelimiterwidget.h"
#include "administratorsettingsdialog/retentionpolicy/retentionpolicysettingswidget.h"
#include "administratorsettingsdialog/slackbridge/slackbridgewidget.h"
#include "administratorsettingsdialog/troubleshoot/troubleshootsettingswidget.h"
#include "administratorsettingsdialog/userdatadownload/userdatadownloadwidget.h"
#include "administratorsettingsdialog/videoconference/videoconferencewidget.h"
#include "administratorsettingsdialog/webdav/webdavsettingswidget.h"
#include "administratorsettingsdialog/webrtc/webrtcsettingswidget.h"
#include <QTabWidget>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorSettingsWidgetTest)

AdministratorSettingsWidgetTest::AdministratorSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void AdministratorSettingsWidgetTest::shouldHaveDefaultValues()
{
    AdministratorSettingsWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTabWidget = w.findChild<QTabWidget *>(u"mTabWidget"_s);
    QVERIFY(mTabWidget);

    auto mAccountSettingsWidget = w.findChild<AccountSettingsWidget *>(u"mAccountSettingsWidget"_s);
    QVERIFY(mAccountSettingsWidget);

    auto mEncryptionSettingsWidget = w.findChild<EncryptionSettingsWidget *>(u"mEncryptionSettingsWidget"_s);
    QVERIFY(mEncryptionSettingsWidget);

    auto mMessageSettingsWidget = w.findChild<MessageSettingsWidget *>(u"mMessageSettingsWidget"_s);
    QVERIFY(mMessageSettingsWidget);

    auto mUploadFileSettingsWidget = w.findChild<FileUploadSettingsWidget *>(u"mUploadFileSettingsWidget"_s);
    QVERIFY(mUploadFileSettingsWidget);

    auto mRetentionPolicySettingsWidget = w.findChild<RetentionPolicySettingsWidget *>(u"mRetentionPolicySettingsWidget"_s);
    QVERIFY(mRetentionPolicySettingsWidget);

    auto mGeneralSettingsWidget = w.findChild<GeneralSettingsWidget *>(u"mGeneralSettingsWidget"_s);
    QVERIFY(mGeneralSettingsWidget);

    auto mRateLimiterWidget = w.findChild<RateLimiterWidget *>(u"mRateLimiterWidget"_s);
    QVERIFY(mRateLimiterWidget);

    auto mPasswordSettingsWidget = w.findChild<PasswordSettingsWidget *>(u"mPasswordSettingsWidget"_s);
    QVERIFY(mPasswordSettingsWidget);

    auto mVideoConferenceWidget = w.findChild<VideoConferenceWidget *>(u"mVideoConferenceWidget"_s);
    QVERIFY(mVideoConferenceWidget);

    auto mIrcFederationWidget = w.findChild<IrcFederationWidget *>(u"mIrcFederationWidget"_s);
    QVERIFY(mIrcFederationWidget);

    auto mWebDavSettingsWidget = w.findChild<WebDavSettingsWidget *>(u"mWebDavSettingsWidget"_s);
    QVERIFY(mWebDavSettingsWidget);

    auto mLDapSettingsWidget = w.findChild<LDapSettingsWidget *>(u"mLDapSettingsWidget"_s);
    QVERIFY(mLDapSettingsWidget);

    auto mLayoutSettingsWidget = w.findChild<LayoutSettingsWidget *>(u"mLayoutSettingsWidget"_s);
    QVERIFY(mLayoutSettingsWidget);

    auto mEnterpriseSettingsWidget = w.findChild<EnterpriseSettingsWidget *>(u"mEnterpriseSettingsWidget"_s);
    QVERIFY(mEnterpriseSettingsWidget);

    auto mUserDataDownloadWidget = w.findChild<UserDataDownloadWidget *>(u"mUserDataDownloadWidget"_s);
    QVERIFY(mUserDataDownloadWidget);

    auto mSlackBridgeWidget = w.findChild<SlackBridgeWidget *>(u"mSlackBridgeWidget"_s);
    QVERIFY(mSlackBridgeWidget);

    auto mLogsSettingsWidget = w.findChild<LogsSettingsWidget *>(u"mLogsSettingsWidget"_s);
    QVERIFY(mLogsSettingsWidget);

    auto mEmailSettingsWidget = w.findChild<EmailSettingsWidget *>(u"mEmailSettingsWidget"_s);
    QVERIFY(mEmailSettingsWidget);

    auto mMobileSettingsWidget = w.findChild<MobileSettingsWidget *>(u"mMobileSettingsWidget"_s);
    QVERIFY(mMobileSettingsWidget);

    auto mTroubleshootSettingsWidget = w.findChild<TroubleshootSettingsWidget *>(u"mTroubleshootSettingsWidget"_s);
    QVERIFY(mTroubleshootSettingsWidget);

    auto mConferenceCallSettingsWidget = w.findChild<ConferenceCallSettingsWidget *>(u"mConferenceCallSettingsWidget"_s);
    QVERIFY(mConferenceCallSettingsWidget);

    auto mWebRtcSettingsWidget = w.findChild<WebRtcSettingsWidget *>(u"mWebRtcSettingsWidget"_s);
    QVERIFY(mWebRtcSettingsWidget);

    auto mCasSettingsWidget = w.findChild<CasSettingsWidget *>(u"mCasSettingsWidget"_s);
    QVERIFY(mCasSettingsWidget);

    auto mOauthSettingsWidget = w.findChild<OauthSettingsWidget *>(u"mOauthSettingsWidget"_s);
    QVERIFY(mOauthSettingsWidget);

    auto mAnalyticsWidget = w.findChild<AnalyticsWidget *>(u"mAnalyticsWidget"_s);
    QVERIFY(mAnalyticsWidget);

    auto mIrcWidget = w.findChild<IrcWidget *>(u"mIrcWidget"_s);
    QVERIFY(mIrcWidget);
}

#include "moc_administratorsettingswidgettest.cpp"
