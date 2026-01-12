/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "featurepreviewpreferencestest.h"
#include "ownuser/featurepreviewpreferences.h"
#include <QTest>
QTEST_GUILESS_MAIN(FeaturePreviewPreferencesTest)

FeaturePreviewPreferencesTest::FeaturePreviewPreferencesTest(QObject *parent)
    : QObject{parent}
{
}

void FeaturePreviewPreferencesTest::shouldHaveDefaultValues()
{
    const FeaturePreviewPreferences preview;
    QCOMPARE(preview.featurePreviewTypes(), FeaturePreviewPreferences::FeaturePreviewType::None);
}

#include "moc_featurepreviewpreferencestest.cpp"
