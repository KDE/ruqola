/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorydialogtest.h"
#include "directory/directorycontainerwidget.h"
#include "directory/directorydialog.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(DirectoryDialogTest)
DirectoryDialogTest::DirectoryDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void DirectoryDialogTest::shouldHaveDefaultValues()
{
    DirectoryDialog d(nullptr, DirectoryWidget::DirectoryType::Room);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mDirectoryContainerWidget = d.findChild<DirectoryContainerWidget *>(QStringLiteral("mDirectoryContainerWidget"));
    QVERIFY(mDirectoryContainerWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_directorydialogtest.cpp"
