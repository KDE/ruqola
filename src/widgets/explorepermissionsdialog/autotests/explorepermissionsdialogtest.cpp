/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "explorepermissionsdialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "explorepermissionsdialog/explorepermissionsdialog.h"
#include "explorepermissionsdialog/explorepermissionswidget.h"
#include <QDialogButtonBox>
#include <QLabel>
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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mExplorePermissionWidget = d.findChild<ExplorePermissionsWidget *>(u"mExplorePermissionWidget"_s);
    QVERIFY(mExplorePermissionWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    auto mOwnRoles = d.findChild<QLabel *>(u"mOwnRoles"_s);
    QVERIFY(mOwnRoles);
    QVERIFY(mOwnRoles->text().isEmpty());
}

#include "moc_explorepermissionsdialogtest.cpp"
