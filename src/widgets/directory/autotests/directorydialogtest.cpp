/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorydialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mDirectoryContainerWidget = d.findChild<DirectoryContainerWidget *>(u"mDirectoryContainerWidget"_s);
    QVERIFY(mDirectoryContainerWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_directorydialogtest.cpp"
