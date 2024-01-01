/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagedialogtest.h"
#include "dialogs/searchmessagedialog.h"
#include "dialogs/searchmessagewidget.h"

#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(SearchMessageDialogTest)
SearchMessageDialogTest::SearchMessageDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void SearchMessageDialogTest::shouldHaveDefaultValues()
{
    SearchMessageDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mSearchChannelWidget = w.findChild<SearchMessageWidget *>(QStringLiteral("mSearchMessageWidget"));
    QVERIFY(mSearchChannelWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_searchmessagedialogtest.cpp"
