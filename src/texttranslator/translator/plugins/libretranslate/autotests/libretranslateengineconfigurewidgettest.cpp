/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "libretranslateengineconfigurewidgettest.h"
#include "../libretranslateengineconfigurewidget.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(LibreTranslateEngineConfigureWidgetTest)
LibreTranslateEngineConfigureWidgetTest::LibreTranslateEngineConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void LibreTranslateEngineConfigureWidgetTest::shouldHaveDefaultValues()
{
    LibreTranslateEngineConfigureWidget w;
    QVERIFY(w.serverUrl().isEmpty());

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mServerUrl = w.findChild<QLineEdit *>(QStringLiteral("mServerUrl"));
    QVERIFY(mServerUrl);
    QVERIFY(mServerUrl->isClearButtonEnabled());

    auto mApiKey = w.findChild<QLineEdit *>(QStringLiteral("mApiKey"));
    QVERIFY(mApiKey);
    QVERIFY(mApiKey->isClearButtonEnabled());

    QVERIFY(w.apiKey().isEmpty());
    QVERIFY(w.serverUrl().isEmpty());
    QVERIFY(!w.serverRequiredApiKey());

    auto mRequiredApiKey = w.findChild<QCheckBox *>(QStringLiteral("mRequiredApiKey"));
    QVERIFY(mRequiredApiKey);
    QVERIFY(!mRequiredApiKey->isChecked());
}
