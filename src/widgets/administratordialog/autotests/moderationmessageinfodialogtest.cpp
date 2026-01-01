/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageinfodialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    ModerationMessageInfoDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mModerationMessageInfoWidget = d.findChild<ModerationMessageInfoWidget *>(u"mModerationMessageInfoWidget"_s);
    QVERIFY(mModerationMessageInfoWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_moderationmessageinfodialogtest.cpp"
