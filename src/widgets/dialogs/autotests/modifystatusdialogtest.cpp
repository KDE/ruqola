/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "modifystatusdialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/modifystatusdialog.h"
#include "dialogs/modifystatuswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ModifyStatusDialogTest)
ModifyStatusDialogTest::ModifyStatusDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ModifyStatusDialogTest::shouldHaveDefaultValues()
{
    ModifyStatusDialog w;
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mModifyStatusWidget = w.findChild<ModifyStatusWidget *>(u"mModifyStatusWidget"_s);
    QVERIFY(mModifyStatusWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    QVERIFY(w.messageStatus().isEmpty());
}

#include "moc_modifystatusdialogtest.cpp"
