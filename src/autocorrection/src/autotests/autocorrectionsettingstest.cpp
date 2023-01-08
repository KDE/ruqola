/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectionsettingstest.h"
#include "autocorrectionsettings.h"
#include <QStandardPaths>
#include <QTest>
QTEST_MAIN(AutoCorrectionSettingsTest)
AutoCorrectionSettingsTest::AutoCorrectionSettingsTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void AutoCorrectionSettingsTest::shouldHaveDefaultValues()
{
    TextAutoCorrection::AutoCorrectionSettings settings;
    QVERIFY(!settings.isEnabledAutoCorrection());
    QVERIFY(!settings.isUppercaseFirstCharOfSentence());
    QVERIFY(!settings.isFixTwoUppercaseChars());
    QVERIFY(!settings.isSingleSpaces());
    QVERIFY(!settings.isAutoFractions());
    QVERIFY(!settings.isCapitalizeWeekDays());
    QVERIFY(!settings.isReplaceDoubleQuotes());
    QVERIFY(!settings.isReplaceSingleQuotes());
    QVERIFY(!settings.isAdvancedAutocorrect());
    QVERIFY(!settings.isAutoFormatUrl());
    QVERIFY(!settings.isAutoBoldUnderline());
    QVERIFY(!settings.isSuperScript());
    QVERIFY(!settings.isReplaceDoubleQuotesByFrenchQuotes());
    QVERIFY(!settings.isAddNonBreakingSpace());
}
