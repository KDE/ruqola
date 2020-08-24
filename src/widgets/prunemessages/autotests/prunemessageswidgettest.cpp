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

#include "prunemessageswidgettest.h"
#include "prunemessages/prunemessageswidget.h"
#include <QCheckBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(PruneMessagesWidgetTest)

PruneMessagesWidgetTest::PruneMessagesWidgetTest(QObject *parent)
    : QObject(parent)
{

}

void PruneMessagesWidgetTest::shouldHaveDefaultValues()
{
    PruneMessagesWidget w;

    QVBoxLayout *mainLayout = w.findChild<QVBoxLayout*>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    QCheckBox *mInclusive = w.findChild<QCheckBox *>(QStringLiteral("mInclusive"));
    QVERIFY(mInclusive);
    QVERIFY(!mInclusive->text().isEmpty());
    QVERIFY(!mInclusive->isChecked());

    QCheckBox *mDoNotPrunePinnedMessage = w.findChild<QCheckBox *>(QStringLiteral("mDoNotPrunePinnedMessage"));
    QVERIFY(mDoNotPrunePinnedMessage);
    QVERIFY(!mDoNotPrunePinnedMessage->text().isEmpty());
    QVERIFY(!mDoNotPrunePinnedMessage->isChecked());

    QCheckBox *mDoNotPruneDiscussionMessage = w.findChild<QCheckBox *>(QStringLiteral("mDoNotPruneDiscussionMessage"));
    QVERIFY(mDoNotPruneDiscussionMessage);
    QVERIFY(!mDoNotPruneDiscussionMessage->text().isEmpty());
    QVERIFY(!mDoNotPruneDiscussionMessage->isChecked());

    QCheckBox *mDoNotPruneThreads = w.findChild<QCheckBox *>(QStringLiteral("mDoNotPruneThreads"));
    QVERIFY(mDoNotPruneThreads);
    QVERIFY(!mDoNotPruneThreads->text().isEmpty());
    QVERIFY(!mDoNotPruneThreads->isChecked());

    QCheckBox *mOnlyRemoveAttachedFiles = w.findChild<QCheckBox *>(QStringLiteral("mOnlyRemoveAttachedFiles"));
    QVERIFY(mOnlyRemoveAttachedFiles);
    QVERIFY(!mOnlyRemoveAttachedFiles->text().isEmpty());
    QVERIFY(!mOnlyRemoveAttachedFiles->isChecked());
}
