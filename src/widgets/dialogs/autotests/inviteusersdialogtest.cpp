/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "inviteusersdialogtest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mInviteUsersWidget = w.findChild<InviteUsersWidget *>(QStringLiteral("mInviteUsersWidget"));
    QVERIFY(mInviteUsersWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
}

#include "moc_inviteusersdialogtest.cpp"
