/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "featurepreviewwidgettest.h"
#include "administratordialog/featurepreview/featurepreviewwidget.h"
#include <QTest>
QTEST_MAIN(FeaturePreviewWidgetTest)

FeaturePreviewWidgetTest::FeaturePreviewWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void FeaturePreviewWidgetTest::shouldHaveDefaultValues()
{
    FeaturePreviewWidget w(nullptr);
    // TODO
}

#include "moc_featurepreviewwidgettest.cpp"
