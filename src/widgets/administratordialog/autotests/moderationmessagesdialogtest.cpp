/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "moderationmessagesdialogtest.h"
#include "administratordialog/moderationconsole/moderationmessagesdialog.h"
#include "administratordialog/moderationconsole/moderationmessageswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ModerationMessagesDialogTest)
ModerationMessagesDialogTest::ModerationMessagesDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ModerationMessagesDialogTest::shouldHaveDefaultValues()
{
    ModerationMessagesDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mModerationMessagesWidget = d.findChild<ModerationMessagesWidget *>(QStringLiteral("mModerationMessagesWidget"));
    QVERIFY(mModerationMessagesWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_moderationmessagesdialogtest.cpp"
