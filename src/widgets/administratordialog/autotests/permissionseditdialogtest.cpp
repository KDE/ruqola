/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionseditdialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/permissions/permissionseditdialog.h"
#include "administratordialog/permissions/permissionseditwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(PermissionsEditDialogTest)
PermissionsEditDialogTest::PermissionsEditDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void PermissionsEditDialogTest::shouldHaveDefaultValues()
{
    PermissionsEditDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mPermissionsWidget = d.findChild<PermissionsEditWidget *>(u"mPermissionsWidget"_s);
    QVERIFY(mPermissionsWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_permissionseditdialogtest.cpp"
