/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorconfigurelanguagelistwidgettest.h"
#include "translator/widgets/translatorconfigurelanguagelistwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TranslatorConfigureLanguageListWidgetTest)
TranslatorConfigureLanguageListWidgetTest::TranslatorConfigureLanguageListWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TranslatorConfigureLanguageListWidgetTest::shouldHaveDefaultValues()
{
    PimCommonTextTranslator::TranslatorConfigureLanguageListWidget w({});
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mLanguageListWidget = w.findChild<QListView *>(QStringLiteral("mLanguageListWidget"));
    QVERIFY(mLanguageListWidget);

    auto mListSearchLine = w.findChild<QLineEdit *>(QStringLiteral("mListSearchLine"));
    QVERIFY(mListSearchLine);
    QVERIFY(!mListSearchLine->placeholderText().isEmpty());

    auto mLabel = w.findChild<QLabel *>(QStringLiteral("mLabel"));
    QVERIFY(mLabel);
    QVERIFY(mLabel->text().isEmpty());
    QVERIFY(w.selectedLanguages().isEmpty());
}

void TranslatorConfigureLanguageListWidgetTest::shouldAssignLabel()
{
    const QString label{QStringLiteral("FOO")};
    PimCommonTextTranslator::TranslatorConfigureLanguageListWidget w(label);
    auto mLabel = w.findChild<QLabel *>(QStringLiteral("mLabel"));
    QVERIFY(mLabel);
    QCOMPARE(mLabel->text(), label);
}
