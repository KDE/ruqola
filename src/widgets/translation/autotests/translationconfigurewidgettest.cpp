/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translationconfigurewidgettest.h"
#include "translation/translationconfigurewidget.h"
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TranslationConfigureWidgetTest)
TranslationConfigureWidgetTest::TranslationConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TranslationConfigureWidgetTest::shouldHaveDefaultValues()
{
    TranslationConfigureWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    // TODO
}
