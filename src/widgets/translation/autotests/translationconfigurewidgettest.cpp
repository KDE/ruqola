/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "translationconfigurewidgettest.h"
#include "translation/translationconfigurewidget.h"
#include <QComboBox>
#include <QLabel>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(TranslationConfigureWidgetTest)
TranslationConfigureWidgetTest::TranslationConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void TranslationConfigureWidgetTest::shouldHaveDefaultValues()
{
    TranslationConfigureWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mEngine = w.findChild<QComboBox *>(QStringLiteral("mEngine"));
    QVERIFY(mEngine);
    // TODO
}
