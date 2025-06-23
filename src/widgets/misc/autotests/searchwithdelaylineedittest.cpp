/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchwithdelaylineedittest.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/searchwithdelaylineedit.h"
#include <QTest>
QTEST_MAIN(SearchWithDelayLineEditTest)
SearchWithDelayLineEditTest::SearchWithDelayLineEditTest(QObject *parent)
    : QObject(parent)
{
}

void SearchWithDelayLineEditTest::shouldHaveDefaultValues()
{
    SearchWithDelayLineEdit w;
    QVERIFY(w.isClearButtonEnabled());
    QVERIFY(!w.placeholderText().isEmpty());
}

#include "moc_searchwithdelaylineedittest.cpp"
