/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionsdialogtest.h"
#include "explorepermissionsdialog/explorepermissionsdialog.h"
#include "explorepermissionsdialog/explorepermissionswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ExplorePermissionsDialogTest)
ExplorePermissionsDialogTest::ExplorePermissionsDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ExplorePermissionsDialogTest::shouldHaveDefaultValues()
{
    ExplorePermissionsDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mExporePermissionWidget = d.findChild<ExplorePermissionsWidget *>(QStringLiteral("mExporePermissionWidget"));
    QVERIFY(mExporePermissionWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}
