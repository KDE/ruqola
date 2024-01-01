/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimagewidgettest.h"
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
    const auto pixmap = QPixmap(QStringLiteral(":/icons/systray.png"));
    info.pixmap = pixmap;
    w.setImageInfo(info);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto graphicsView = w.findChild<ImageGraphicsView *>(QStringLiteral("mImageGraphicsView"));
    QVERIFY(graphicsView);

    auto zoomLayout = w.findChild<QHBoxLayout *>(QStringLiteral("zoomLayout"));
    QVERIFY(zoomLayout);

    auto zoomLabel = w.findChild<QLabel *>(QStringLiteral("zoomLabel"));
    QVERIFY(zoomLabel);
    QVERIFY(!zoomLabel->text().isEmpty());

    auto mZoomSpin = w.findChild<QDoubleSpinBox *>(QStringLiteral("mZoomSpin"));
    QVERIFY(mZoomSpin);
    QCOMPARE(mZoomSpin->value(), 1.0);
    QCOMPARE(mZoomSpin->singleStep(), 0.1);

    auto mSlider = w.findChild<QSlider *>(QStringLiteral("mSlider"));
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

    auto resetButton = w.findChild<QPushButton *>(QStringLiteral("resetButton"));
    QVERIFY(resetButton);
    resetButton->click();
    QCOMPARE(graphicsView->zoom(), (qreal)1.0);
    QCOMPARE(mZoomSpin->value(), 1.0);
    QCOMPARE(mSlider->value(), 100);

    QVERIFY(!w.imageInfo().isAnimatedImage);
    QVERIFY(!w.imageInfo().pixmap.isNull());
}

#include "moc_showimagewidgettest.cpp"
