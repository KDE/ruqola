/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "deeplengineconfiguredialogtest.h"
#include "../deeplengineconfiguredialog.h"
#include "../deeplengineconfigurewidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(DeeplEngineConfigureDialogTest)
DeeplEngineConfigureDialogTest::DeeplEngineConfigureDialogTest(QObject *parent)
    : QObject{parent}
{
}

void DeeplEngineConfigureDialogTest::shouldHaveDefaultValues()
{
    DeeplEngineConfigureDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    auto mConfigureWidget = d.findChild<DeeplEngineConfigureWidget *>(QStringLiteral("mConfigureWidget"));
    QVERIFY(mConfigureWidget);

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto buttonBox = d.findChild<QDialogButtonBox *>(QStringLiteral("buttonBox"));
    QVERIFY(buttonBox);
}
