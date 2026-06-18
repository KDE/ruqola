/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showbannedusersdialogtest.h"

#include "dialogs/showbannedusersdialog.h"
#include "dialogs/showbanneduserswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ShowBannedUsersDialogTest)
using namespace Qt::Literals::StringLiterals;
ShowBannedUsersDialogTest::ShowBannedUsersDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ShowBannedUsersDialogTest::shouldHaveDefaultValues()
{
    ShowBannedUsersDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mShowBannedUsersWidget = w.findChild<ShowBannedUsersWidget *>(u"mShowBannedUsersWidget"_s);
    QVERIFY(mShowBannedUsersWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_showbannedusersdialogtest.cpp"
