/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatormenutest.h"
#include "translator/translatormenu.h"
#include <QStandardPaths>
#include <QTest>

QTEST_MAIN(TranslatorMenuTest)
TranslatorMenuTest::TranslatorMenuTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void TranslatorMenuTest::shouldHaveDefaultValues()
{
    TranslatorMenu w;
    QVERIFY(w.menu());
    QVERIFY(w.isEmpty());
}
