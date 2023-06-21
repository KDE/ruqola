/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomemojicreateorupdatedialogtest.h"
#include "administratordialog/customemoji/administratorcustomemojicreateorupdatedialog.h"
#include "administratordialog/customemoji/administratorcustomemojicreateorupdatewidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorCustomEmojiCreateOrUpdateDialogTest)

AdministratorCustomEmojiCreateOrUpdateDialogTest::AdministratorCustomEmojiCreateOrUpdateDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void AdministratorCustomEmojiCreateOrUpdateDialogTest::shouldHaveDefaultValues()
{
    AdministratorCustomEmojiCreateOrUpdateDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCreateWidget = w.findChild<AdministratorCustomEmojiCreateOrUpdateWidget *>(QStringLiteral("mCreateWidget"));
    QVERIFY(mCreateWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
}

#include "moc_administratorcustomemojicreateorupdatedialogtest.cpp"
