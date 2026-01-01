/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showattachmentdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mShowAttachmentWidget = w.findChild<ShowAttachmentWidget *>(u"mShowAttachmentWidget"_s);
    QVERIFY(mShowAttachmentWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_showattachmentdialogtest.cpp"
