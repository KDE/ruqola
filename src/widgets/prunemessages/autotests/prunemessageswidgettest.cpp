/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "prunemessageswidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto lastestLayout = w.findChild<QHBoxLayout *>(u"lastestLayout"_s);
    QVERIFY(lastestLayout);
    QCOMPARE(lastestLayout->contentsMargins(), QMargins{});

    auto lastestLabel = w.findChild<QLabel *>(u"lastestLabel"_s);
    QVERIFY(lastestLabel);
    QVERIFY(!lastestLabel->text().isEmpty());

    auto mLastestDateTimeEdit = w.findChild<QDateTimeEdit *>(u"mLastestDateTimeEdit"_s);
    QVERIFY(mLastestDateTimeEdit);

    auto oldestLayout = w.findChild<QHBoxLayout *>(u"oldestLayout"_s);
    QVERIFY(oldestLayout);
    QCOMPARE(oldestLayout->contentsMargins(), QMargins{});

    auto oldestLabel = w.findChild<QLabel *>(u"oldestLabel"_s);
    QVERIFY(oldestLabel);
    QVERIFY(!oldestLabel->text().isEmpty());

    auto mOldestDateTimeEdit = w.findChild<QDateTimeEdit *>(u"mLastestDateTimeEdit"_s);
    QVERIFY(mOldestDateTimeEdit);

    auto usersLabel = w.findChild<QLabel *>(u"usersLabel"_s);
    QVERIFY(usersLabel);
    QVERIFY(!usersLabel->text().isEmpty());
    QVERIFY(usersLabel->wordWrap());
    QCOMPARE(usersLabel->textFormat(), Qt::PlainText);

    auto mUsers = w.findChild<AddUsersWidget *>(u"mUsers"_s);
    QVERIFY(mUsers);
    mainLayout->addWidget(mUsers);

    auto mInclusive = w.findChild<QCheckBox *>(u"mInclusive"_s);
    QVERIFY(mInclusive);
    QVERIFY(!mInclusive->text().isEmpty());
    QVERIFY(!mInclusive->isChecked());

    auto mDoNotPrunePinnedMessage = w.findChild<QCheckBox *>(u"mDoNotPrunePinnedMessage"_s);
    QVERIFY(mDoNotPrunePinnedMessage);
    QVERIFY(!mDoNotPrunePinnedMessage->text().isEmpty());
    QVERIFY(!mDoNotPrunePinnedMessage->isChecked());

    auto mDoNotPruneDiscussionMessage = w.findChild<QCheckBox *>(u"mDoNotPruneDiscussionMessage"_s);
    QVERIFY(mDoNotPruneDiscussionMessage);
    QVERIFY(!mDoNotPruneDiscussionMessage->text().isEmpty());
    QVERIFY(!mDoNotPruneDiscussionMessage->isChecked());

    auto mDoNotPruneThreads = w.findChild<QCheckBox *>(u"mDoNotPruneThreads"_s);
    QVERIFY(mDoNotPruneThreads);
    QVERIFY(!mDoNotPruneThreads->text().isEmpty());
    QVERIFY(!mDoNotPruneThreads->isChecked());

    auto mOnlyRemoveAttachedFiles = w.findChild<QCheckBox *>(u"mOnlyRemoveAttachedFiles"_s);
    QVERIFY(mOnlyRemoveAttachedFiles);
    QVERIFY(!mOnlyRemoveAttachedFiles->text().isEmpty());
    QVERIFY(!mOnlyRemoveAttachedFiles->isChecked());

    auto mInfoLabel = w.findChild<QLabel *>(u"mInfoLabel"_s);
    QVERIFY(mInfoLabel);
    QVERIFY(mInfoLabel->text().isEmpty());
    QVERIFY(mInfoLabel->wordWrap());
}

#include "moc_prunemessageswidgettest.cpp"
