/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussionsmodeltest.h"
#include "discussions/discussions.h"
#include "model/discussionsmodel.h"
#include "test_model_helpers.h"
#include <QTest>
QTEST_GUILESS_MAIN(DiscussionsModelTest)
DiscussionsModelTest::DiscussionsModelTest(QObject *parent)
    : QObject(parent)
{
}

void DiscussionsModelTest::shouldHaveDefaultValues()
{
    DiscussionsModel model;
    QVERIFY(!model.loadMoreDiscussionsInProgress());
}

void DiscussionsModelTest::shouldAssignValues()
{
    DiscussionsModel w;
    QSignalSpy rowInsertedSpy(&w, &DiscussionsModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &DiscussionsModel::rowsAboutToBeInserted);
    QSignalSpy modelAboutToBeResetSpy(&w, &DiscussionsModel::modelAboutToBeReset);

    Discussions discussionList;
    for (int i = 0; i < 10; ++i) {
        Discussion c;
        c.setDescription(QStringLiteral("roomid%1").arg(i));
        c.setNumberMessages(i);
        c.setParentRoomId(QStringLiteral("online"));
        discussionList.append(std::move(c));
    }
    w.setDiscussions(discussionList);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(modelAboutToBeResetSpy.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,9"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,9"));

    // add Empty list
    discussionList.clear();
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    modelAboutToBeResetSpy.clear();

    w.setDiscussions(discussionList);

    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(modelAboutToBeResetSpy.count(), 1);

    // Add same element
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    modelAboutToBeResetSpy.clear();

    w.setDiscussions(discussionList);

    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(modelAboutToBeResetSpy.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QString());

    // Test add same number of element.
    discussionList.clear();
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    modelAboutToBeResetSpy.clear();

    for (int i = 0; i < 5; ++i) {
        Discussion c;
        c.setDescription(QStringLiteral("roomid%1").arg(i));
        c.setNumberMessages(i);
        c.setParentRoomId(QStringLiteral("online"));
        discussionList.append(std::move(c));
    }
    w.setDiscussions(discussionList);
    QCOMPARE(w.rowCount(), 5);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(modelAboutToBeResetSpy.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,4"));

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    modelAboutToBeResetSpy.clear();
    w.setDiscussions(discussionList);

    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(modelAboutToBeResetSpy.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,4"));
}

#include "moc_discussionsmodeltest.cpp"
