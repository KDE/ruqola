/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingsaskapplicationwidgettest.h"
#include "applicationssettingsdialog/applicationssettingsaskapplicationwidget.h"
#include <KTextEdit>
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ApplicationsSettingsAskApplicationWidgetTest)
ApplicationsSettingsAskApplicationWidgetTest::ApplicationsSettingsAskApplicationWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void ApplicationsSettingsAskApplicationWidgetTest::shouldHaveDefaultValues()
{
    ApplicationsSettingsAskApplicationWidget d;
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto label = d.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mTextAppLabel = d.findChild<QLabel *>(QStringLiteral("mTextAppLabel"));
    QVERIFY(mTextAppLabel);
    QVERIFY(mTextAppLabel->text().isEmpty());

    auto mMessage = d.findChild<KTextEdit *>(QStringLiteral("mMessage"));
    QVERIFY(mMessage);
    QVERIFY(mMessage->toPlainText().isEmpty());
    QVERIFY(!mMessage->placeholderText().isEmpty());
}

#include "moc_applicationssettingsaskapplicationwidgettest.cpp"
