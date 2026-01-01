/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewdiscussionwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/channelsearchnamelineedit.h"
#include "dialogs/channelsearchwidget.h"
#include "dialogs/createnewdiscussionwidget.h"
#include "misc/adduserswidget.h"
#include <KTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(CreateNewDiscussionWidgetTest)

CreateNewDiscussionWidgetTest::CreateNewDiscussionWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void CreateNewDiscussionWidgetTest::shouldHaveDefaultValues()
{
    CreateNewDiscussionWidget w(nullptr);
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto channelLabel = w.findChild<QLabel *>(u"channelLabel"_s);
    QVERIFY(channelLabel);
    QVERIFY(!channelLabel->text().isEmpty());

    auto mChannelNameLineEdit = w.findChild<ChannelSearchWidget *>(u"mChannelNameLineEdit"_s);
    QVERIFY(mChannelNameLineEdit);

    auto discussionName = w.findChild<QLabel *>(u"discussionName"_s);
    QVERIFY(discussionName);
    QVERIFY(!discussionName->text().isEmpty());

    auto mDiscussionNameLineEdit = w.findChild<QLineEdit *>(u"mDiscussionNameLineEdit"_s);
    QVERIFY(mDiscussionNameLineEdit);
    QVERIFY(mDiscussionNameLineEdit->text().isEmpty());
    QVERIFY(mDiscussionNameLineEdit->isClearButtonEnabled());

    auto usersLabel = w.findChild<QLabel *>(u"usersLabel"_s);
    QVERIFY(usersLabel);
    QVERIFY(!usersLabel->text().isEmpty());

    auto mUsers = w.findChild<AddUsersWidget *>(u"mUsers"_s);
    QVERIFY(mUsers);
    QVERIFY(mUsers->userIds().isEmpty());

    auto messageLabel = w.findChild<QLabel *>(u"messageLabel"_s);
    QVERIFY(messageLabel);
    QVERIFY(!messageLabel->text().isEmpty());

    auto mMessageTextEdit = w.findChild<KTextEdit *>(u"mMessageTextEdit"_s);
    QVERIFY(mMessageTextEdit);
    QVERIFY(mMessageTextEdit->toPlainText().isEmpty());
    QVERIFY(!mMessageTextEdit->acceptRichText());
}

#include "moc_createnewdiscussionwidgettest.cpp"
