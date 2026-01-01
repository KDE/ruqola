/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "moderationusersdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mModerationUsersWidget = d.findChild<ModerationUsersWidget *>(u"mModerationUsersWidget"_s);
    QVERIFY(mModerationUsersWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_moderationusersdialogtest.cpp"
