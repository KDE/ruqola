/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "ruqolamainwindowtest.h"
#include "misc/statuscombobox.h"
#include "ruqolamainwindow.h"

#include <QLabel>
#include <QSplitter>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QTest>
#include <QToolButton>

QTEST_MAIN(RuqolaMainWindowTest)

RuqolaMainWindowTest::RuqolaMainWindowTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
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

    auto mStatusComboBox = w.findChild<StatusCombobox *>(QStringLiteral("mStatusComboBox"));
    QVERIFY(mStatusComboBox);

    auto mStatusBarTypingMessage = w.findChild<QLabel *>(QStringLiteral("mStatusBarTypingMessage"));
    QVERIFY(mStatusBarTypingMessage);
    QCOMPARE(mStatusBarTypingMessage->textFormat(), Qt::RichText);

    auto mNotificationToolButton = w.findChild<QToolButton *>(QStringLiteral("mNotificationToolButton"));
    QVERIFY(mNotificationToolButton);
    QVERIFY(!mNotificationToolButton->toolTip().isEmpty());
    QVERIFY(mNotificationToolButton->isHidden());
    QVERIFY(mNotificationToolButton->autoRaise());
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

#include "moc_ruqolamainwindowtest.cpp"
