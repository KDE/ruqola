/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewimagetest.h"
#include "autogenerateui/autogenerateinteractionuiviewimage.h"
#include <QTest>
QTEST_MAIN(AutoGenerateInteractionUiViewImageTest)

AutoGenerateInteractionUiViewImageTest::AutoGenerateInteractionUiViewImageTest(QObject *parent)
    : QObject{parent}
{
}

void AutoGenerateInteractionUiViewImageTest::shouldHaveDefaultValues()
{
    AutoGenerateInteractionUiViewImage img;
    QVERIFY(img.imageUrl().isEmpty());
    QVERIFY(img.altText().isEmpty());
}

#include "moc_autogenerateinteractionuiviewimagetest.cpp"
