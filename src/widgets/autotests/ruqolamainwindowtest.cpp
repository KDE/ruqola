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

#include "ruqolamainwindowtest.h"
#include "ruqolamainwindow.h"

#include <QSplitter>
#include <QStackedWidget>
#include <QTest>

QTEST_MAIN(RuqolaMainWindowTest)

RuqolaMainWindowTest::RuqolaMainWindowTest(QObject *parent)
    : QObject(parent)
{
}

static void switchToMainWidget(RuqolaMainWindow &w)
{
    auto *mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);
    auto *mRuqolaMainWidget = mStackedWidget->findChild<QWidget *>(QStringLiteral("mRuqolaMainWidget"));
    QVERIFY(mRuqolaMainWidget);
    mStackedWidget->setCurrentWidget(mRuqolaMainWidget);
    QCOMPARE(mStackedWidget->currentWidget(), mRuqolaMainWidget);
}

void RuqolaMainWindowTest::shouldHaveDefaultValues()
{
    RuqolaMainWindow w;
    auto *mSplitter = w.findChild<QSplitter *>(QStringLiteral("mSplitter"));
    QVERIFY(mSplitter);
    switchToMainWidget(w);
}

void RuqolaMainWindowTest::shouldRestoreSizes()
{
    // Save
    QList<int> actualSizes;
    {
        RuqolaMainWindow w;
        switchToMainWidget(w);
        w.resize(500, 500);
        w.show();

        auto *mSplitter = w.findChild<QSplitter *>(QStringLiteral("mSplitter"));
        QVERIFY(mSplitter);
        mSplitter->setSizes({100, 400});
        actualSizes = mSplitter->sizes(); // not exactly {100, 400} but more something like {167, 308}
    }
    // Restore
    {
        RuqolaMainWindow w;
        switchToMainWidget(w);
        QCOMPARE(w.size(), QSize(500, 500));
        w.show();

        auto *mSplitter = w.findChild<QSplitter *>(QStringLiteral("mSplitter"));
        QVERIFY(mSplitter);
        QCOMPARE(mSplitter->sizes(), actualSizes);
    }
}
