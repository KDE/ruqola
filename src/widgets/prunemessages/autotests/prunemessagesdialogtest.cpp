/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "prunemessagesdialogtest.h"
#include "prunemessages/prunemessagesdialog.h"
#include "prunemessages/prunemessageswidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(PruneMessagesDialogTest)

PruneMessagesDialogTest::PruneMessagesDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void PruneMessagesDialogTest::shouldHaveDefaultValues()
{
    PruneMessagesDialog d(nullptr);
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mPruneMessageWidget = d.findChild<PruneMessagesWidget *>(QStringLiteral("mPruneMessageWidget"));
    QVERIFY(mPruneMessageWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}
