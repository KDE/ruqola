/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorsettingswidgettest.h"
#include "administratorsettingsdialog/accounts/accountsettingswidget.h"
#include "administratorsettingsdialog/administratorsettingswidget.h"
#include "administratorsettingsdialog/encryption/encryptionsettingswidget.h"
#include "administratorsettingsdialog/fileupload/fileuploadsettingswidget.h"
#include "administratorsettingsdialog/general/generalsettingswidget.h"
#include "administratorsettingsdialog/message/messagesettingswidget.h"
#include "administratorsettingsdialog/password/passwordsettingswidget.h"
#include "administratorsettingsdialog/ratelimiter/ratelimiterwidget.h"
#include "administratorsettingsdialog/retentionpolicy/retentionpolicysettingswidget.h"
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
}
