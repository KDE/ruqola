/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussionsmodeltest.h"
#include "discussions/discussions.h"
#include "model/discussionsmodel.h"
#include <QSignalSpy>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(DiscussionsModelTest)
DiscussionsModelTest::DiscussionsModelTest(QObject *parent)
    : QObject(parent)
{
}

void DiscussionsModelTest::shouldHaveDefaultValues()
{
    const DiscussionsModel model;
    QVERIFY(!model.loadMoreDiscussionsInProgress());
}

void DiscussionsModelTest::shouldAssignValues()
{
    DiscussionsModel w;
    QSignalSpy rowInsertedSpy(&w, &DiscussionsModel::rowsInserted);
    QSignalSpy modelAboutToBeResetSpy(&w, &DiscussionsModel::modelAboutToBeReset);
    QSignalSpy modelResetSpy(&w, &DiscussionsModel::modelReset);

    Discussions discussionList;
    for (int i = 0; i < 10; ++i) {
        Discussion c;
        c.setDescription(u"roomid%1"_s.arg(i));
        c.setNumberMessages(i);
        c.setParentRoomId("online"_ba);
        discussionList.append(c);
    }
    w.setDiscussions(discussionList);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(modelAboutToBeResetSpy.count(), 1);
    QCOMPARE(modelResetSpy.count(), 1);

    // add Empty list
    discussionList.clear();
    rowInsertedSpy.clear();
    modelAboutToBeResetSpy.clear();
    modelResetSpy.clear();

    w.setDiscussions(discussionList);

    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(modelAboutToBeResetSpy.count(), 1);
    QCOMPARE(modelResetSpy.count(), 1);

    // Add same element
    rowInsertedSpy.clear();
    modelAboutToBeResetSpy.clear();
    modelResetSpy.clear();

    w.setDiscussions(discussionList);

    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(modelAboutToBeResetSpy.count(), 1);
    QCOMPARE(modelResetSpy.count(), 1);

    // Test add same number of element.
    discussionList.clear();
    rowInsertedSpy.clear();
    modelAboutToBeResetSpy.clear();
    modelResetSpy.clear();

    for (int i = 0; i < 5; ++i) {
        Discussion c;
        c.setDescription(u"roomid%1"_s.arg(i));
        c.setNumberMessages(i);
        c.setParentRoomId("online"_ba);
        discussionList.append(c);
    }
    w.setDiscussions(discussionList);
    QCOMPARE(w.rowCount(), 5);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(modelAboutToBeResetSpy.count(), 1);
    QCOMPARE(modelResetSpy.count(), 1);

    rowInsertedSpy.clear();
    modelAboutToBeResetSpy.clear();
    modelResetSpy.clear();
    w.setDiscussions(discussionList);

    QCOMPARE(w.rowCount(), 5);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(modelAboutToBeResetSpy.count(), 1);
    QCOMPARE(modelResetSpy.count(), 1);
}

#include "moc_discussionsmodeltest.cpp"
