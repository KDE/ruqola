/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "prunemessageswidgettest.h"
#include "misc/adduserswidget.h"
#include "prunemessages/prunemessageswidget.h"
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(PruneMessagesWidgetTest)

PruneMessagesWidgetTest::PruneMessagesWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void PruneMessagesWidgetTest::shouldHaveDefaultValues()
{
    PruneMessagesWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto lastestLayout = w.findChild<QHBoxLayout *>(QStringLiteral("lastestLayout"));
    QVERIFY(lastestLayout);
    QCOMPARE(lastestLayout->contentsMargins(), QMargins{});

    auto lastestLabel = w.findChild<QLabel *>(QStringLiteral("lastestLabel"));
    QVERIFY(lastestLabel);
    QVERIFY(!lastestLabel->text().isEmpty());

    auto mLastestDateTimeEdit = w.findChild<QDateTimeEdit *>(QStringLiteral("mLastestDateTimeEdit"));
    QVERIFY(mLastestDateTimeEdit);

    auto oldestLayout = w.findChild<QHBoxLayout *>(QStringLiteral("oldestLayout"));
    QVERIFY(oldestLayout);
    QCOMPARE(oldestLayout->contentsMargins(), QMargins{});

    auto oldestLabel = w.findChild<QLabel *>(QStringLiteral("oldestLabel"));
    QVERIFY(oldestLabel);
    QVERIFY(!oldestLabel->text().isEmpty());

    auto mOldestDateTimeEdit = w.findChild<QDateTimeEdit *>(QStringLiteral("mLastestDateTimeEdit"));
    QVERIFY(mOldestDateTimeEdit);

    auto usersLabel = w.findChild<QLabel *>(QStringLiteral("usersLabel"));
    QVERIFY(usersLabel);
    QVERIFY(!usersLabel->text().isEmpty());
    QVERIFY(usersLabel->wordWrap());
    QCOMPARE(usersLabel->textFormat(), Qt::PlainText);

    auto mUsers = w.findChild<AddUsersWidget *>(QStringLiteral("mUsers"));
    QVERIFY(mUsers);
    mainLayout->addWidget(mUsers);

    auto mInclusive = w.findChild<QCheckBox *>(QStringLiteral("mInclusive"));
    QVERIFY(mInclusive);
    QVERIFY(!mInclusive->text().isEmpty());
    QVERIFY(!mInclusive->isChecked());

    auto mDoNotPrunePinnedMessage = w.findChild<QCheckBox *>(QStringLiteral("mDoNotPrunePinnedMessage"));
    QVERIFY(mDoNotPrunePinnedMessage);
    QVERIFY(!mDoNotPrunePinnedMessage->text().isEmpty());
    QVERIFY(!mDoNotPrunePinnedMessage->isChecked());

    auto mDoNotPruneDiscussionMessage = w.findChild<QCheckBox *>(QStringLiteral("mDoNotPruneDiscussionMessage"));
    QVERIFY(mDoNotPruneDiscussionMessage);
    QVERIFY(!mDoNotPruneDiscussionMessage->text().isEmpty());
    QVERIFY(!mDoNotPruneDiscussionMessage->isChecked());

    auto mDoNotPruneThreads = w.findChild<QCheckBox *>(QStringLiteral("mDoNotPruneThreads"));
    QVERIFY(mDoNotPruneThreads);
    QVERIFY(!mDoNotPruneThreads->text().isEmpty());
    QVERIFY(!mDoNotPruneThreads->isChecked());

    auto mOnlyRemoveAttachedFiles = w.findChild<QCheckBox *>(QStringLiteral("mOnlyRemoveAttachedFiles"));
    QVERIFY(mOnlyRemoveAttachedFiles);
    QVERIFY(!mOnlyRemoveAttachedFiles->text().isEmpty());
    QVERIFY(!mOnlyRemoveAttachedFiles->isChecked());

    auto mInfoLabel = w.findChild<QLabel *>(QStringLiteral("mInfoLabel"));
    QVERIFY(mInfoLabel);
    QVERIFY(mInfoLabel->text().isEmpty());
    QVERIFY(mInfoLabel->wordWrap());
}

#include "moc_prunemessageswidgettest.cpp"
