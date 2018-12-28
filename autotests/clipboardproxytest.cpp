/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "clipboardproxytest.h"
#include "clipboardproxy.h"
#include <QTest>
#include <QSignalSpy>

QTEST_MAIN(ClipboardProxyTest)

ClipboardProxyTest::ClipboardProxyTest(QObject *parent)
    : QObject(parent)
{
}

void ClipboardProxyTest::shouldEmitSignals()
{
    ClipboardProxy w;
    QSignalSpy dataChangedSpy(&w, &ClipboardProxy::dataChanged);
    QSignalSpy selectionChangedSpy(&w, &ClipboardProxy::selectionChanged);

    w.setDataText(QStringLiteral("foo"));
    QCOMPARE(w.dataText(), QStringLiteral("foo"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QCOMPARE(selectionChangedSpy.count(), 0);

    dataChangedSpy.clear();
    selectionChangedSpy.clear();

    //Same data
    w.setDataText(QStringLiteral("foo"));
    QCOMPARE(w.dataText(), QStringLiteral("foo"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QCOMPARE(selectionChangedSpy.count(), 0);

    dataChangedSpy.clear();
    selectionChangedSpy.clear();

    w.setSelectionText(QStringLiteral("foo"));
    QCOMPARE(w.selectionText(), QStringLiteral("foo"));
    QCOMPARE(dataChangedSpy.count(), 0);
    QCOMPARE(selectionChangedSpy.count(), 1);
}
