/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorconfiguredialogtest.h"
#include "translator/widgets/translatorconfiguredialog.h"
#include "translator/widgets/translatorconfigurewidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(TranslatorConfigureDialogTest)
TranslatorConfigureDialogTest::TranslatorConfigureDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void TranslatorConfigureDialogTest::shouldHaveDefaultValues()
{
    TextTranslator::TranslatorConfigureDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mTranslatorConfigureWidget = w.findChild<TextTranslator::TranslatorConfigureWidget *>(QStringLiteral("mTranslatorConfigureWidget"));
    QVERIFY(mTranslatorConfigureWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("buttonBox"));
    QVERIFY(buttonBox);
}
