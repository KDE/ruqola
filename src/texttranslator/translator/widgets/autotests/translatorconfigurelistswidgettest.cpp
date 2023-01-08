/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorconfigurelistswidgettest.h"
#include "translator/widgets/translatorconfigurecombowidget.h"
#include "translator/widgets/translatorconfigurelanguagelistwidget.h"
#include "translator/widgets/translatorconfigurelistswidget.h"
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(TranslatorConfigureListsWidgetTest)
TranslatorConfigureListsWidgetTest::TranslatorConfigureListsWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void TranslatorConfigureListsWidgetTest::shouldHaveDefaultValues()
{
    TextTranslator::TranslatorConfigureListsWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mEngineConfigureComboWidget = w.findChild<TextTranslator::TranslatorConfigureComboWidget *>(QStringLiteral("mEngineConfigureComboWidget"));
    QVERIFY(mEngineConfigureComboWidget);

    auto mFromLanguageWidget = w.findChild<TextTranslator::TranslatorConfigureLanguageListWidget *>(QStringLiteral("mFromLanguageWidget"));
    QVERIFY(mFromLanguageWidget);

    auto mToLanguageWidget = w.findChild<TextTranslator::TranslatorConfigureLanguageListWidget *>(QStringLiteral("mToLanguageWidget"));
    QVERIFY(mToLanguageWidget);
}
