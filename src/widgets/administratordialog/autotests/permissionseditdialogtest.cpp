/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "permissionseditdialogtest.h"
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

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mPermissionsWidget = d.findChild<PermissionsEditWidget *>(QStringLiteral("mPermissionsWidget"));
    QVERIFY(mPermissionsWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_permissionseditdialogtest.cpp"
