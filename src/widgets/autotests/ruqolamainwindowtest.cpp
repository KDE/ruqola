/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolamainwindowtest.h"
#include "misc/statuscombobox.h"
#include "ruqolamainwindow.h"

#include <QLabel>
#include <QSplitter>
#include <QStackedWidget>
#include <QTest>
#include <QWidgetAction>

QTEST_MAIN(RuqolaMainWindowTest)

RuqolaMainWindowTest::RuqolaMainWindowTest(QObject *parent)
    : QObject(parent)
{
}

static void switchToMainWidget(RuqolaMainWindow &w)
{
    auto mStackedWidget = w.findChild<QStackedWidget *>(QStringLiteral("mStackedWidget"));
    QVERIFY(mStackedWidget);
    auto mRuqolaMainWidget = mStackedWidget->findChild<QWidget *>(QStringLiteral("mRuqolaMainWidget"));
    QVERIFY(mRuqolaMainWidget);
    mStackedWidget->setCurrentWidget(mRuqolaMainWidget);
    QCOMPARE(mStackedWidget->currentWidget(), mRuqolaMainWidget);
}

void RuqolaMainWindowTest::shouldHaveDefaultValues()
{
    RuqolaMainWindow w;
    auto mSplitter = w.findChild<QSplitter *>(QStringLiteral("mSplitter"));
    QVERIFY(mSplitter);
    switchToMainWidget(w);

    auto statusAction = qobject_cast<QWidgetAction *>(w.action("status"));
    QVERIFY(statusAction);
    QVERIFY(!statusAction->text().isEmpty());

    auto status = statusAction->defaultWidget();
    QVERIFY(status);
    QVERIFY(status->layout());

    auto mStatusComboBox = status->findChild<StatusCombobox *>(QStringLiteral("mStatusComboBox"));
    QVERIFY(mStatusComboBox);

    auto label = status->findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
}

void RuqolaMainWindowTest::shouldRestoreSizes()
{
    // Save
    QList<int> actualSizes;
    {
        RuqolaMainWindow w;
        switchToMainWidget(w);
        w.resize(800, 800);
        w.show();

        auto mSplitter = w.findChild<QSplitter *>(QStringLiteral("mSplitter"));
        QVERIFY(mSplitter);
        mSplitter->setSizes({100, 400});
        actualSizes = mSplitter->sizes(); // not exactly {100, 400} but more something like {167, 308}
    }
    // Restore
    {
        RuqolaMainWindow w;
        switchToMainWidget(w);
        QCOMPARE(w.size(), QSize(800, 800));
        w.show();

        auto mSplitter = w.findChild<QSplitter *>(QStringLiteral("mSplitter"));
        QVERIFY(mSplitter);
        QCOMPARE(mSplitter->sizes(), actualSizes);
    }
}
