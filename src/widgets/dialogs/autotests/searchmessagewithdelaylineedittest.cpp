/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagewithdelaylineedittest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/searchmessagewithdelaylineedit.h"
#include <QCompleter>
#include <QStringListModel>
#include <QTest>
QTEST_MAIN(SearchMessageWithDelayLineEditTest)

SearchMessageWithDelayLineEditTest::SearchMessageWithDelayLineEditTest(QObject *parent)
    : QObject{parent}
{
}

void SearchMessageWithDelayLineEditTest::shouldHaveDefaultValues()
{
    SearchMessageWithDelayLineEdit w(nullptr);
    auto mCompleter = w.findChild<QCompleter *>(u"mCompleter"_s);
    QVERIFY(mCompleter);

    auto mCompleterListModel = w.findChild<QStringListModel *>(u"mCompleterListModel"_s);
    QVERIFY(mCompleterListModel);
    QCOMPARE(mCompleterListModel->rowCount(), 0);
}

#include "moc_searchmessagewithdelaylineedittest.cpp"
