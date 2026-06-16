/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "featurepreviewmessagewidgettest.h"
#include "administratordialog/featurepreview/featurepreviewmessagewidget.h"
#include <QTest>
QTEST_MAIN(FeaturePreviewMessageWidgetTest)

FeaturePreviewMessageWidgetTest::FeaturePreviewMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void FeaturePreviewMessageWidgetTest::shouldHaveDefaultValues()
{
    const FeaturePreviewMessageWidget w;
    QCOMPARE(w.messageType(), KMessageWidget::Information);
    QCOMPARE(w.position(), KMessageWidget::Footer);
}

#include "moc_featurepreviewmessagewidgettest.cpp"
