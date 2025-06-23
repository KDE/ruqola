/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "moderationmessagesdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mModerationMessagesWidget = d.findChild<ModerationMessagesWidget *>(u"mModerationMessagesWidget"_s);
    QVERIFY(mModerationMessagesWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_moderationmessagesdialogtest.cpp"
