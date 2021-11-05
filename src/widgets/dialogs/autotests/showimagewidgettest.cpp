/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
    ShowImageWidget w;
    const auto pixmap = QPixmap(QStringLiteral(":/icons/systray.png"));
    w.setImage(pixmap);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), {});

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

    QVERIFY(!w.isAnimatedPixmap());

    auto resetButton = w.findChild<QPushButton *>(QStringLiteral("resetButton"));
    QVERIFY(resetButton);
    resetButton->click();
    QCOMPARE(graphicsView->zoom(), (qreal)1.0);
    QCOMPARE(mZoomSpin->value(), 1.0);
    QCOMPARE(mSlider->value(), 100);
}
