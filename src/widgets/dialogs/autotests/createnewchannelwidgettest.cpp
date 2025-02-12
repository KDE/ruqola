/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewchannelwidgettest.h"
#include "dialogs/channelnamevalidlinewidget.h"
#include "dialogs/createnewchannelwidget.h"
#include "misc/adduserswidget.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(CreateNewChannelWidgetTest)
CreateNewChannelWidgetTest::CreateNewChannelWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void CreateNewChannelWidgetTest::shouldHaveDefaultValues()
{
    CreateNewChannelWidget w(nullptr);
    auto mChannelName = w.findChild<ChannelNameValidLineWidget *>(QStringLiteral("mChannelName"));
    QVERIFY(mChannelName);

    auto mTopicLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mTopicLineEdit"));
    QVERIFY(mTopicLineEdit);

    auto mUsers = w.findChild<AddUsersWidget *>(QStringLiteral("mUsers"));
    QVERIFY(mUsers);

    auto mReadOnly = w.findChild<QCheckBox *>(QStringLiteral("mReadOnly"));
    QVERIFY(mReadOnly);
    QVERIFY(!mReadOnly->toolTip().isEmpty());
    auto mBroadcast = w.findChild<QCheckBox *>(QStringLiteral("mBroadcast"));
    QVERIFY(mBroadcast);
    QVERIFY(!mBroadcast->toolTip().isEmpty());
    auto mPrivate = w.findChild<QCheckBox *>(QStringLiteral("mPrivate"));
    QVERIFY(mPrivate);
    QVERIFY(!mPrivate->toolTip().isEmpty());

    auto mEncryptedRoom = w.findChild<QCheckBox *>(QStringLiteral("mEncryptedRoom"));
    QVERIFY(mEncryptedRoom);
    QVERIFY(!mEncryptedRoom->toolTip().isEmpty());

    auto mFederated = w.findChild<QCheckBox *>(QStringLiteral("mFederated"));
    QVERIFY(mFederated);
}

#include "moc_createnewchannelwidgettest.cpp"
