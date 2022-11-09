/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingswidgettest.h"
#include "administratorsettingsdialog/accounts/accountsettingswidget.h"
#include "administratorsettingsdialog/administratorsettingswidget.h"
#include "administratorsettingsdialog/email/emailsettingswidget.h"
#include "administratorsettingsdialog/encryption/encryptionsettingswidget.h"
#include "administratorsettingsdialog/enterprise/enterprisesettingswidget.h"
#include "administratorsettingsdialog/fileupload/fileuploadsettingswidget.h"
#include "administratorsettingsdialog/general/generalsettingswidget.h"
#include "administratorsettingsdialog/ircfederation/ircfederationwidget.h"
#include "administratorsettingsdialog/layout/layoutsettingswidget.h"
#include "administratorsettingsdialog/ldap/ldapsettingswidget.h"
#include "administratorsettingsdialog/logs/logssettingswidget.h"
#include "administratorsettingsdialog/message/messagesettingswidget.h"
#include "administratorsettingsdialog/mobile/mobilesettingswidget.h"
#include "administratorsettingsdialog/password/passwordsettingswidget.h"
#include "administratorsettingsdialog/ratelimiter/ratelimiterwidget.h"
#include "administratorsettingsdialog/retentionpolicy/retentionpolicysettingswidget.h"
#include "administratorsettingsdialog/slackbridge/slackbridgewidget.h"
#include "administratorsettingsdialog/userdatadownload/userdatadownloadwidget.h"
#include "administratorsettingsdialog/videoconference/videoconferencewidget.h"
#include "administratorsettingsdialog/webdav/webdavsettingswidget.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mTabWidget = w.findChild<QTabWidget *>(QStringLiteral("mTabWidget"));
    QVERIFY(mTabWidget);

    auto mAccountSettingsWidget = w.findChild<AccountSettingsWidget *>(QStringLiteral("mAccountSettingsWidget"));
    QVERIFY(mAccountSettingsWidget);

    auto mEncryptionSettingsWidget = w.findChild<EncryptionSettingsWidget *>(QStringLiteral("mEncryptionSettingsWidget"));
    QVERIFY(mEncryptionSettingsWidget);

    auto mMessageSettingsWidget = w.findChild<MessageSettingsWidget *>(QStringLiteral("mMessageSettingsWidget"));
    QVERIFY(mMessageSettingsWidget);

    auto mUploadFileSettingsWidget = w.findChild<FileUploadSettingsWidget *>(QStringLiteral("mUploadFileSettingsWidget"));
    QVERIFY(mUploadFileSettingsWidget);

    auto mRetentionPolicySettingsWidget = w.findChild<RetentionPolicySettingsWidget *>(QStringLiteral("mRetentionPolicySettingsWidget"));
    QVERIFY(mRetentionPolicySettingsWidget);

    auto mGeneralSettingsWidget = w.findChild<GeneralSettingsWidget *>(QStringLiteral("mGeneralSettingsWidget"));
    QVERIFY(mGeneralSettingsWidget);

    auto mRateLimiterWidget = w.findChild<RateLimiterWidget *>(QStringLiteral("mRateLimiterWidget"));
    QVERIFY(mRateLimiterWidget);

    auto mPasswordSettingsWidget = w.findChild<PasswordSettingsWidget *>(QStringLiteral("mPasswordSettingsWidget"));
    QVERIFY(mPasswordSettingsWidget);

    auto mVideoConferenceWidget = w.findChild<VideoConferenceWidget *>(QStringLiteral("mVideoConferenceWidget"));
    QVERIFY(mVideoConferenceWidget);

    auto mIrcFederationWidget = w.findChild<IrcFederationWidget *>(QStringLiteral("mIrcFederationWidget"));
    QVERIFY(mIrcFederationWidget);

    auto mWebDavSettingsWidget = w.findChild<WebDavSettingsWidget *>(QStringLiteral("mWebDavSettingsWidget"));
    QVERIFY(mWebDavSettingsWidget);

    auto mLDapSettingsWidget = w.findChild<LDapSettingsWidget *>(QStringLiteral("mLDapSettingsWidget"));
    QVERIFY(mLDapSettingsWidget);

    auto mLayoutSettingsWidget = w.findChild<LayoutSettingsWidget *>(QStringLiteral("mLayoutSettingsWidget"));
    QVERIFY(mLayoutSettingsWidget);

    auto mEnterpriseSettingsWidget = w.findChild<EnterpriseSettingsWidget *>(QStringLiteral("mEnterpriseSettingsWidget"));
    QVERIFY(mEnterpriseSettingsWidget);

    auto mUserDataDownloadWidget = w.findChild<UserDataDownloadWidget *>(QStringLiteral("mUserDataDownloadWidget"));
    QVERIFY(mUserDataDownloadWidget);

    auto mSlackBridgeWidget = w.findChild<SlackBridgeWidget *>(QStringLiteral("mSlackBridgeWidget"));
    QVERIFY(mSlackBridgeWidget);

    auto mLogsSettingsWidget = w.findChild<LogsSettingsWidget *>(QStringLiteral("mLogsSettingsWidget"));
    QVERIFY(mLogsSettingsWidget);

    auto mEmailSettingsWidget = w.findChild<EmailSettingsWidget *>(QStringLiteral("mEmailSettingsWidget"));
    QVERIFY(mEmailSettingsWidget);

    auto mMobileSettingsWidget = w.findChild<MobileSettingsWidget *>(QStringLiteral("mMobileSettingsWidget"));
    QVERIFY(mMobileSettingsWidget);
}
