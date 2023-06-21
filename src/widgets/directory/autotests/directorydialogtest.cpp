/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorydialogtest.h"
#include "directory/directorydialog.h"
#include "directory/directorytabwidget.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(DirectoryDialogTest)
DirectoryDialogTest::DirectoryDialogTest(QObject *parent)
    : QObject(parent)
{
}

void DirectoryDialogTest::shouldHaveDefaultValues()
{
    DirectoryDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mDirectoryTabWidget = d.findChild<DirectoryTabWidget *>(QStringLiteral("mDirectoryTabWidget"));
    QVERIFY(mDirectoryTabWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_directorydialogtest.cpp"
