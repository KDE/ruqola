/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translatorconfigurewidgettest.h"
#include "translator/translatorconfigurelanguagelistwidget.h"
#include "translator/translatorconfigurewidget.h"
#include <QComboBox>
#include <QLabel>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(TranslatorConfigureWidgetTest)
TranslatorConfigureWidgetTest::TranslatorConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void TranslatorConfigureWidgetTest::shouldHaveDefaultValues()
{
    TranslatorConfigureWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mEngine = w.findChild<QComboBox *>(QStringLiteral("mEngine"));
    QVERIFY(mEngine);

    auto mFromLanguageWidget = w.findChild<TranslatorConfigureLanguageListWidget *>(QStringLiteral("mFromLanguageWidget"));
    QVERIFY(mFromLanguageWidget);

    auto mToLanguageWidget = w.findChild<TranslatorConfigureLanguageListWidget *>(QStringLiteral("mToLanguageWidget"));
    QVERIFY(mToLanguageWidget);
}
