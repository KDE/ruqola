/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showattachmentdialogtest.h"
#include "dialogs/showattachmentdialog.h"
#include "dialogs/showattachmentwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ShowAttachmentDialogTest)
ShowAttachmentDialogTest::ShowAttachmentDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ShowAttachmentDialogTest::shouldHaveDefaultValues()
{
    ShowAttachmentDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mShowAttachmentWidget = w.findChild<ShowAttachmentWidget *>(QStringLiteral("mShowAttachmentWidget"));
    QVERIFY(mShowAttachmentWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_showattachmentdialogtest.cpp"
