/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "discussiontest.h"
#include "discussions/discussion.h"
#include "ruqola_autotest_helper.h"
#include <QJsonObject>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(DiscussionTest)
DiscussionTest::DiscussionTest(QObject *parent)
    : QObject(parent)
{
}

void DiscussionTest::shouldHaveDefaultValue()
{
    Discussion discussion;
    QVERIFY(discussion.description().isEmpty());
    QVERIFY(discussion.parentRoomId().isEmpty());
    QCOMPARE(discussion.numberMessages(), 0);
    QCOMPARE(discussion.lastMessage(), -1);
    QVERIFY(discussion.discussionRoomId().isEmpty());
    QCOMPARE(discussion.timeStamp(), -1);
    QVERIFY(discussion.timeStampDisplay().isEmpty());
    QVERIFY(discussion.fname().isEmpty());
    QVERIFY(discussion.userName().isEmpty());
}

void DiscussionTest::shouldLoadDiscussion_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<Discussion>("discussion");

    QTest::addRow("empty") << u"discussion-empty"_s << Discussion();
    Discussion discussion;

    discussion.setNumberMessages(0);
    discussion.setDiscussionRoomId("652e69986a344bea24b4ce55"_ba);
    discussion.setTimeStamp(1697540504599);
    discussion.setUserName(u"bla"_s);
    QTest::addRow("one-discussion") << u"one-discussion"_s << discussion;
}

void DiscussionTest::shouldLoadDiscussion()
{
    QFETCH(QString, name);
    QFETCH(Discussion, discussion);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/discussion/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    Discussion m;
    m.parseDiscussion(obj);
    QCOMPARE(m, discussion);
}

#include "moc_discussiontest.cpp"
