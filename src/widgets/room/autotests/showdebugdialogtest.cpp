/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showdebugdialogtest.h"
#include "room/debugdialog/showdebugdialog.h"
#include "room/debugdialog/showdebugwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ShowDebugDialogTest)
using namespace Qt::Literals::StringLiterals;
ShowDebugDialogTest::ShowDebugDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ShowDebugDialogTest::shouldHaveDefaultValues()
{
    const ShowDebugDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mShowDebugWidget = d.findChild<ShowDebugWidget *>(u"mShowDebugWidget"_s);
    QVERIFY(mShowDebugWidget);

    auto buttonBox = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(buttonBox);
    QCOMPARE(buttonBox->standardButtons(), {QDialogButtonBox::Close});
}

#include "moc_showdebugdialogtest.cpp"
