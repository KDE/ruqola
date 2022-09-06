/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translationconfigurelanguagelistwidgettest.h"
#include "translation/translationconfigurelanguagelistwidget.h"
#include <KListWidgetSearchLine>
#include <QLabel>
#include <QListWidget>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TranslationConfigureLanguageListWidgetTest)
TranslationConfigureLanguageListWidgetTest::TranslationConfigureLanguageListWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void TranslationConfigureLanguageListWidgetTest::shouldHaveDefaultValues()
{
    TranslationConfigureLanguageListWidget w({});
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mLanguageListWidget = w.findChild<QListWidget *>(QStringLiteral("mLanguageListWidget"));
    QVERIFY(mLanguageListWidget);
    QCOMPARE(mLanguageListWidget->count(), 0);

    auto mListSearchLine = w.findChild<KListWidgetSearchLine *>(QStringLiteral("mListSearchLine"));
    QVERIFY(mListSearchLine);
    QVERIFY(!mListSearchLine->placeholderText().isEmpty());
    QCOMPARE(mListSearchLine->listWidget(), mLanguageListWidget);

    auto mLabel = w.findChild<QLabel *>(QStringLiteral("mLabel"));
    QVERIFY(mLabel);
    QVERIFY(mLabel->text().isEmpty());
    QVERIFY(w.selectedLanguages().isEmpty());
}

void TranslationConfigureLanguageListWidgetTest::shouldAssignLabel()
{
    const QString label{QStringLiteral("FOO")};
    TranslationConfigureLanguageListWidget w(label);
    auto mLabel = w.findChild<QLabel *>(QStringLiteral("mLabel"));
    QVERIFY(mLabel);
    QCOMPARE(mLabel->text(), label);
}
