/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translationconfigurelanguagelistwidgettest.h"
#include "translation/translationconfigurelanguagelistwidget.h"
#include <QTest>
QTEST_MAIN(TranslationConfigureLanguageListWidgetTest)
TranslationConfigureLanguageListWidgetTest::TranslationConfigureLanguageListWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TranslationConfigureLanguageListWidgetTest::shouldHaveDefaultValues()
{
    TranslationConfigureLanguageListWidget w;
    // TODO
}
