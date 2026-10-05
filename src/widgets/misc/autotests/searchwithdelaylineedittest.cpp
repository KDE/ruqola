/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchwithdelaylineedittest.h"

#include "misc/searchwithdelaylineedit.h"
#include <QTest>

using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(SearchWithDelayLineEditTest)
SearchWithDelayLineEditTest::SearchWithDelayLineEditTest(QObject *parent)
    : QObject(parent)
{
}

void SearchWithDelayLineEditTest::shouldHaveDefaultValues()
{
    const SearchWithDelayLineEdit w;
    QVERIFY(w.isClearButtonEnabled());
    QVERIFY(!w.placeholderText().isEmpty());
}

#include "moc_searchwithdelaylineedittest.cpp"
