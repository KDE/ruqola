/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lingvaenginedialogtest.h"
#include "../lingvaenginedialog.h"
#include "../lingvaenginewidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(LingvaEngineDialogTest)
LingvaEngineDialogTest::LingvaEngineDialogTest(QObject *parent)
    : QObject{parent}
{
}

void LingvaEngineDialogTest::shouldHaveDefaultValues()
{
    LingvaEngineDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    auto mConfigureWidget = d.findChild<LingvaEngineWidget *>(QStringLiteral("mConfigureWidget"));
    QVERIFY(mConfigureWidget);

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto buttonBox = d.findChild<QDialogButtonBox *>(QStringLiteral("buttonBox"));
    QVERIFY(buttonBox);
}
