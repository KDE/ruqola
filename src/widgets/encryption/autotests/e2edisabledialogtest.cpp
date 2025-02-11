/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edisabledialogtest.h"
#include "encryption/e2edisabledialog.h"
#include "encryption/e2edisablewidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(E2eDisableDialogTest)

E2eDisableDialogTest::E2eDisableDialogTest(QObject *parent)
    : QObject{parent}
{
}

E2eDisableDialogTest::~E2eDisableDialogTest() = default;

void E2eDisableDialogTest::shouldHaveDefaultValues()
{
    E2eDisableDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mE2eDisableWidget = d.findChild<E2eDisableWidget *>(QStringLiteral("mE2eDisableWidget"));
    QVERIFY(mE2eDisableWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_e2edisabledialogtest.cpp"
