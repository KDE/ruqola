/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "libretranslateengineconfigurewidgettest.h"
#include "../libretranslateengineconfigurewidget.h"
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

    auto mServerUrl = w.findChild<QLineEdit *>(QStringLiteral("mServerUrl"));
    QVERIFY(mServerUrl);
}
