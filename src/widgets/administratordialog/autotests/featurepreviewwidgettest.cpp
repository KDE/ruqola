/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "featurepreviewwidgettest.h"
#include "administratordialog/featurepreview/featurepreviewwidget.h"
#include <QCheckBox>
#include <QTest>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(FeaturePreviewWidgetTest)

FeaturePreviewWidgetTest::FeaturePreviewWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void FeaturePreviewWidgetTest::shouldHaveDefaultValues()
{
    FeaturePreviewWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAllowFeaturePreview = w.findChild<QCheckBox *>(u"mAllowFeaturePreview"_s);
    QVERIFY(mAllowFeaturePreview);

    auto mQuickReactions = w.findChild<QCheckBox *>(u"mQuickReactions"_s);
    QVERIFY(mQuickReactions);
}

#include "moc_featurepreviewwidgettest.cpp"
