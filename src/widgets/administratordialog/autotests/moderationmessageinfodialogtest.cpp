/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageinfodialogtest.h"
#include "administratordialog/moderationconsole/moderationmessageinfodialog.h"
#include "administratordialog/moderationconsole/moderationmessageinfowidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ModerationMessageInfoDialogTest)
ModerationMessageInfoDialogTest::ModerationMessageInfoDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ModerationMessageInfoDialogTest::shouldHaveDefaultValues()
{
    ModerationMessageInfoDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mModerationMessageInfoWidget = d.findChild<ModerationMessageInfoWidget *>(QStringLiteral("mModerationMessageInfoWidget"));
    QVERIFY(mModerationMessageInfoWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_moderationmessageinfodialogtest.cpp"
