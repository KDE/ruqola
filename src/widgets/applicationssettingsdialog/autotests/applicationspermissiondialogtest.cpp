/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "applicationspermissiondialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "applicationssettingsdialog/applicationspermissiondialog.h"
#include "applicationssettingsdialog/applicationspermissionwidget.h"

#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QVBoxLayout>

#include <QTest>

QTEST_MAIN(ApplicationsPermissionDialogTest)

ApplicationsPermissionDialogTest::ApplicationsPermissionDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ApplicationsPermissionDialogTest::shouldHaveDefaultValues()
{
    ApplicationsPermissionDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
    auto mApplicationsPermissionWidget = d.findChild<ApplicationsPermissionWidget *>(u"mApplicationsPermissionWidget"_s);
    QVERIFY(mApplicationsPermissionWidget);
}

#include "moc_applicationspermissiondialogtest.cpp"
