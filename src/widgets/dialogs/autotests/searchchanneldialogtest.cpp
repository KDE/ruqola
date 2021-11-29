/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchchanneldialogtest.h"
#include "dialogs/searchchanneldialog.h"
#include "dialogs/searchchannelwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(SearchChannelDialogTest)
SearchChannelDialogTest::SearchChannelDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void SearchChannelDialogTest::shouldHaveDefaultValues()
{
    SearchChannelDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mSearchChannelWidget = w.findChild<SearchChannelWidget *>(QStringLiteral("mSearchChannelWidget"));
    QVERIFY(mSearchChannelWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}
