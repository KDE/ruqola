/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "myaccountfeaturepreviewconfigurewidgettest.h"
#include "myaccount/myaccountfeaturepreviewconfigurewidget.h"
#include <QTest>
QTEST_MAIN(MyAccountFeaturePreviewConfigureWidgetTest)

MyAccountFeaturePreviewConfigureWidgetTest::MyAccountFeaturePreviewConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MyAccountFeaturePreviewConfigureWidgetTest::shouldHaveDefaultValues()
{
    const MyAccountFeaturePreviewConfigureWidget w(nullptr);
    // TODO
}

#include "moc_myaccountfeaturepreviewconfigurewidgettest.cpp"
