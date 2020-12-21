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

#include "createnewdiscussionwidgettest.h"
#include "dialogs/createnewdiscussionwidget.h"
#include "misc/adduserswidget.h"
#include <QLineEdit>
#include <QLabel>
#include <QTest>
#include <KTextEdit>
#include <QVBoxLayout>

QTEST_MAIN(CreateNewDiscussionWidgetTest)

CreateNewDiscussionWidgetTest::CreateNewDiscussionWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void CreateNewDiscussionWidgetTest::shouldHaveDefaultValues()
{
    CreateNewDiscussionWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto channelLabel = w.findChild<QLabel *>(QStringLiteral("channelLabel"));
    QVERIFY(channelLabel);
    QVERIFY(!channelLabel->text().isEmpty());

    auto mChannelNameLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mChannelNameLineEdit"));
    QVERIFY(mChannelNameLineEdit);
    QVERIFY(mChannelNameLineEdit->text().isEmpty());
    QVERIFY(mChannelNameLineEdit->isClearButtonEnabled());

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
