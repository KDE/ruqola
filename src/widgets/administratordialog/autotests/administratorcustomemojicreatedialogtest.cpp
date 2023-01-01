/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomemojicreatedialogtest.h"
#include "administratordialog/customemoji/administratorcustomemojicreatedialog.h"
#include "administratordialog/customemoji/administratorcustomemojicreatewidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(AdministratorCustomEmojiCreateDialogTest)

AdministratorCustomEmojiCreateDialogTest::AdministratorCustomEmojiCreateDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void AdministratorCustomEmojiCreateDialogTest::shouldHaveDefaultValues()
{
    AdministratorCustomEmojiCreateDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCreateWidget = w.findChild<AdministratorCustomEmojiCreateWidget *>(QStringLiteral("mCreateWidget"));
    QVERIFY(mCreateWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
}
