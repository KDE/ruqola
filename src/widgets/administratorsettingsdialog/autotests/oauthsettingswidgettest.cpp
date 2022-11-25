/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "oauthsettingswidgettest.h"
#include "administratorsettingsdialog/oauth/oauthsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(OauthSettingsWidgetTest)
OauthSettingsWidgetTest::OauthSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void OauthSettingsWidgetTest::shouldHaveDefaultValues()
{
    OauthSettingsWidget w(nullptr);
    auto mSignWithApple = w.findChild<QCheckBox *>(QStringLiteral("mSignWithApple"));
    QVERIFY(mSignWithApple);
    QVERIFY(!mSignWithApple->isChecked());
    QVERIFY(mSignWithApple->text().isEmpty());
    QVERIFY(mSignWithApple->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSignWithApple), QStringLiteral("Accounts_OAuth_Apple"));

    auto mAccountsOAuthAppleId = w.findChild<QLineEdit *>(QStringLiteral("mAccountsOAuthAppleId"));
    QVERIFY(mAccountsOAuthAppleId);
    QVERIFY(mAccountsOAuthAppleId->text().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mAccountsOAuthAppleId), QStringLiteral("Accounts_OAuth_Apple_id"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("Accounts_OAuth_Apple_id"));
}
