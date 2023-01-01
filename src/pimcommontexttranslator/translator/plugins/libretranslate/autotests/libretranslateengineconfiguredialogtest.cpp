/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "libretranslateengineconfiguredialogtest.h"
#include "../libretranslateengineconfiguredialog.h"
#include "../libretranslateengineconfigurewidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(LibreTranslateEngineConfigureDialogTest)
LibreTranslateEngineConfigureDialogTest::LibreTranslateEngineConfigureDialogTest(QObject *parent)
    : QObject{parent}
{
}

void LibreTranslateEngineConfigureDialogTest::shouldHaveDefaultValues()
{
    LibreTranslateEngineConfigureDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    auto mConfigureWidget = d.findChild<LibreTranslateEngineConfigureWidget *>(QStringLiteral("mConfigureWidget"));
    QVERIFY(mConfigureWidget);

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto buttonBox = d.findChild<QDialogButtonBox *>(QStringLiteral("buttonBox"));
    QVERIFY(buttonBox);
}
