/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emailsettingswidgettest.h"
#include "administratorsettingsdialog/email/emailsettingswidget.h"
#include "settingswidgetshelper.h"
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTest>
QTEST_MAIN(EmailSettingsWidgetTest)
EmailSettingsWidgetTest::EmailSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void EmailSettingsWidgetTest::shouldHaveDefaultValues()
{
    EmailSettingsWidget w(nullptr);
    auto smtpLabel = w.findChild<QLabel *>(QStringLiteral("smtpLabel"));
    QVERIFY(smtpLabel);
    QVERIFY(!smtpLabel->text().isEmpty());

    auto mSmtpHost = w.findChild<QLineEdit *>(QStringLiteral("mSmtpHost"));
    QVERIFY(mSmtpHost);
    QVERIFY(mSmtpHost->text().isEmpty());
    QVERIFY(mSmtpHost->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSmtpHost), QStringLiteral("SMTP_Host"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("SMTP_Host"));

    auto mSmtpPort = w.findChild<QSpinBox *>(QStringLiteral("mSmtpPort"));
    QVERIFY(mSmtpPort);
    QVERIFY(mSmtpPort->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mSmtpPort), QStringLiteral("SMTP_Port"));
    SettingsWidgetHelper::checkLabelToolButton(&w, QStringLiteral("SMTP_Port"));

    auto mIgnoreTls = w.findChild<QCheckBox *>(QStringLiteral("mIgnoreTls"));
    QVERIFY(mIgnoreTls);
    QVERIFY(!mIgnoreTls->isChecked());
    QVERIFY(!mIgnoreTls->text().isEmpty());
    QVERIFY(mIgnoreTls->toolTip().isEmpty());
    QCOMPARE(SettingsWidgetHelper::widgetSettingsName(mIgnoreTls), QStringLiteral("SMTP_IgnoreTLS"));
}
