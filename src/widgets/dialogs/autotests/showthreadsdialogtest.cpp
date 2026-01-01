/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showthreadsdialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/showtheadscombobox.h"
#include "dialogs/showthreadsdialog.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ShowThreadsDialogTest)

ShowThreadsDialogTest::ShowThreadsDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ShowThreadsDialogTest::shouldHaveDefaultValues()
{
    ShowThreadsDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    auto mShowTheadsComboBox = w.findChild<ShowTheadsComboBox *>(u"mShowTheadsComboBox"_s);
    QVERIFY(mShowTheadsComboBox);
}

#include "moc_showthreadsdialogtest.cpp"
