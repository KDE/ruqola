/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "deeplengineconfigurewidgettest.h"
#include "../deeplengineconfigurewidget.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(DeeplEngineConfigureWidgetTest)
DeeplEngineConfigureWidgetTest::DeeplEngineConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void DeeplEngineConfigureWidgetTest::shouldHaveDefaultValues()
{
    DeeplEngineConfigureWidget w;
    QVERIFY(!w.useFreeLicenceKey());

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mUseFreeLicense = w.findChild<QCheckBox *>(QStringLiteral("mUseFreeLicense"));
    QVERIFY(mUseFreeLicense);
    QVERIFY(!mUseFreeLicense->isChecked());
}
