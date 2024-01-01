/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewdiscussionwidgettest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto channelLabel = w.findChild<QLabel *>(QStringLiteral("channelLabel"));
    QVERIFY(channelLabel);
    QVERIFY(!channelLabel->text().isEmpty());

    auto mChannelNameLineEdit = w.findChild<ChannelSearchWidget *>(QStringLiteral("mChannelNameLineEdit"));
    QVERIFY(mChannelNameLineEdit);

    auto discussionName = w.findChild<QLabel *>(QStringLiteral("discussionName"));
    QVERIFY(discussionName);
    QVERIFY(!discussionName->text().isEmpty());

    auto mDiscussionNameLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mDiscussionNameLineEdit"));
    QVERIFY(mDiscussionNameLineEdit);
    QVERIFY(mDiscussionNameLineEdit->text().isEmpty());
    QVERIFY(mDiscussionNameLineEdit->isClearButtonEnabled());

    auto usersLabel = w.findChild<QLabel *>(QStringLiteral("usersLabel"));
    QVERIFY(usersLabel);
    QVERIFY(!usersLabel->text().isEmpty());

    auto mUsers = w.findChild<AddUsersWidget *>(QStringLiteral("mUsers"));
    QVERIFY(mUsers);
    QVERIFY(mUsers->userIds().isEmpty());

    auto messageLabel = w.findChild<QLabel *>(QStringLiteral("messageLabel"));
    QVERIFY(messageLabel);
    QVERIFY(!messageLabel->text().isEmpty());

    auto mMessageTextEdit = w.findChild<KTextEdit *>(QStringLiteral("mMessageTextEdit"));
    QVERIFY(mMessageTextEdit);
    QVERIFY(mMessageTextEdit->toPlainText().isEmpty());
    QVERIFY(!mMessageTextEdit->acceptRichText());
}

#include "moc_createnewdiscussionwidgettest.cpp"
