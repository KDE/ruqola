/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showreadreceiptsdialogtest.h"
#include "dialogs/showreadreceiptsdialog.h"
#include "dialogs/showreadreceiptswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ShowReadReceiptsDialogTest)
using namespace Qt::Literals::StringLiterals;
ShowReadReceiptsDialogTest::ShowReadReceiptsDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ShowReadReceiptsDialogTest::shouldHaveDefaultValues()
{
    ShowReadReceiptsDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mShowReadReceiptsWidget = w.findChild<ShowReadReceiptsWidget *>(u"mShowReadReceiptsWidget"_s);
    QVERIFY(mShowReadReceiptsWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}
#include "moc_showreadreceiptsdialogtest.cpp"
