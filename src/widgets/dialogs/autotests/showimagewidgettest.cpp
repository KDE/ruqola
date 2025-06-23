/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimagewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/showimagegraphicsview.h"
#include "dialogs/showimagewidget.h"

#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ShowImageWidgetTest)

ShowImageWidgetTest::ShowImageWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ShowImageWidgetTest::shouldHaveDefaultValues()
{
    ShowImageWidget w(nullptr);
    ShowImageWidget::ImageInfo info;
    const auto pixmap = QPixmap(u":/icons/systray.png"_s);
    info.pixmap = pixmap;
    w.setImageInfo(info);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto graphicsView = w.findChild<ShowImageGraphicsView *>(u"mImageGraphicsView"_s);
    QVERIFY(graphicsView);

    auto zoomLayout = w.findChild<QHBoxLayout *>(u"zoomLayout"_s);
    QVERIFY(zoomLayout);

    auto zoomLabel = w.findChild<QLabel *>(u"zoomLabel"_s);
    QVERIFY(zoomLabel);
    QVERIFY(!zoomLabel->text().isEmpty());

    auto mZoomSpin = w.findChild<QDoubleSpinBox *>(u"mZoomSpin"_s);
    QVERIFY(mZoomSpin);
    QCOMPARE(mZoomSpin->value(), 1.0);
    QCOMPARE(mZoomSpin->singleStep(), 0.1);

    auto mSlider = w.findChild<QSlider *>(u"mSlider"_s);
    QVERIFY(mSlider);
    QCOMPARE(mSlider->orientation(), Qt::Horizontal);
    QCOMPARE(mSlider->value(), 100);

    mSlider->setValue(200);
    QCOMPARE(mSlider->value(), 200);
    QCOMPARE(mZoomSpin->value(), 2);
    QCOMPARE(graphicsView->zoom(), (qreal)2.0);

    mZoomSpin->setValue(3);
    QCOMPARE(mZoomSpin->value(), 3);
    QCOMPARE(mSlider->value(), 300);
    QCOMPARE(graphicsView->zoom(), (qreal)3.0);

    auto resetButton = w.findChild<QPushButton *>(u"resetButton"_s);
    QVERIFY(resetButton);
    resetButton->click();
    QCOMPARE(graphicsView->zoom(), (qreal)1.0);
    QCOMPARE(mZoomSpin->value(), 1.0);
    QCOMPARE(mSlider->value(), 100);

    QVERIFY(!w.imageInfo().isAnimatedImage);
    QVERIFY(!w.imageInfo().pixmap.isNull());
}

#include "moc_showimagewidgettest.cpp"
