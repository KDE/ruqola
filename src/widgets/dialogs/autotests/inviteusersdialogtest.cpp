/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inviteusersdialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/inviteusersdialog.h"
#include "dialogs/inviteuserswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(InviteUsersDialogTest)

InviteUsersDialogTest::InviteUsersDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void InviteUsersDialogTest::shouldHaveDefaultValues()
{
    InviteUsersDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mInviteUsersWidget = w.findChild<InviteUsersWidget *>(u"mInviteUsersWidget"_s);
    QVERIFY(mInviteUsersWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(buttonBox);
}

#include "moc_inviteusersdialogtest.cpp"
