/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "moderationusersdialogtest.h"
#include "administratordialog/moderationconsole/moderationusersdialog.h"
#include "administratordialog/moderationconsole/moderationuserswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ModerationUsersDialogTest)
ModerationUsersDialogTest::ModerationUsersDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ModerationUsersDialogTest::shouldHaveDefaultValues()
{
    ModerationUsersDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mModerationUsersWidget = d.findChild<ModerationUsersWidget *>(QStringLiteral("mModerationUsersWidget"));
    QVERIFY(mModerationUsersWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_moderationusersdialogtest.cpp"
