/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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
#include <QLabel>
#include <QScrollArea>
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

    auto *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto *scrollArea = w.findChild<QScrollArea *>(QStringLiteral("scrollArea"));
    QVERIFY(scrollArea);
    QVERIFY(scrollArea->widgetResizable());

    auto *mLabel = w.findChild<QLabel *>(QStringLiteral("mLabel"));
    QVERIFY(mLabel);
    QVERIFY(mLabel->text().isEmpty());
    QCOMPARE(mLabel->backgroundRole(), QPalette::Base);

    auto *zoomLayout = w.findChild<QHBoxLayout *>(QStringLiteral("zoomLayout"));
    QVERIFY(zoomLayout);

    auto *zoomLabel = w.findChild<QLabel *>(QStringLiteral("zoomLabel"));
    QVERIFY(zoomLabel);
    QVERIFY(!zoomLabel->text().isEmpty());

    auto *mSlider = w.findChild<QSlider *>(QStringLiteral("mSlider"));
    QVERIFY(mSlider);
    QCOMPARE(mSlider->orientation(), Qt::Horizontal);
}
