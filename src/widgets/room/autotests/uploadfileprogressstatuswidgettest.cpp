/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "uploadfileprogressstatuswidgettest.h"
#include "room/uploadfileprogressstatuswidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QTest>
QTEST_MAIN(UploadFileProgressStatusWidgetTest)
UploadFileProgressStatusWidgetTest::UploadFileProgressStatusWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void UploadFileProgressStatusWidgetTest::shouldHaveDefaultValues()
{
    UploadFileProgressStatusWidget w;

    auto hboxLayout = w.findChild<QHBoxLayout *>(QStringLiteral("hboxLayout"));
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), {});

    auto mProgressBar = w.findChild<QProgressBar *>(QStringLiteral("mProgressBar"));
    QVERIFY(mProgressBar);
    QCOMPARE(mProgressBar->minimum(), 0);
    QCOMPARE(mProgressBar->maximum(), 100);

    auto mFileName = w.findChild<QLabel *>(QStringLiteral("mFileName"));
    QVERIFY(mFileName);
    QVERIFY(mFileName->text().isEmpty());
}

void UploadFileProgressStatusWidgetTest::shouldChangeText()
{
    UploadFileProgressStatusWidget w;
    const QString text = QStringLiteral("bla");
    auto mFileName = w.findChild<QLabel *>(QStringLiteral("mFileName"));
    QVERIFY(mFileName->text().isEmpty());

    w.setUploadFileName(text);
    QCOMPARE(mFileName->text(), text);
}
