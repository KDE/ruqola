/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "administratorcustomemojicreatewidgettest.h"
#include "administratordialog/customemoji/administratorcustomemojicreatewidget.h"

#include <KUrlRequester>
#include <QFormLayout>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(AdministratorCustomEmojiCreateWidgetTest)
AdministratorCustomEmojiCreateWidgetTest::AdministratorCustomEmojiCreateWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void AdministratorCustomEmojiCreateWidgetTest::shouldHaveDefaultValues()
{
    AdministratorCustomEmojiCreateWidget w;
    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

    auto mName = w.findChild<QLineEdit *>(QStringLiteral("mName"));
    QVERIFY(mName);
    QVERIFY(mName->text().isEmpty());
    QVERIFY(mName->isClearButtonEnabled());

    auto mAlias = w.findChild<QLineEdit *>(QStringLiteral("mAlias"));
    QVERIFY(mAlias);
    QVERIFY(mAlias->text().isEmpty());
    QVERIFY(mAlias->isClearButtonEnabled());

    auto mSelectFile = w.findChild<KUrlRequester *>(QStringLiteral("mSelectFile"));
    QVERIFY(mSelectFile);
}
