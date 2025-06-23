/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "e2edisabledialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mE2eDisableWidget = d.findChild<E2eDisableWidget *>(u"mE2eDisableWidget"_s);
    QVERIFY(mE2eDisableWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_e2edisabledialogtest.cpp"
