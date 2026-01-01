/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewchannelwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mChannelName = w.findChild<ChannelNameValidLineWidget *>(u"mChannelName"_s);
    QVERIFY(mChannelName);

    auto mTopicLineEdit = w.findChild<QLineEdit *>(u"mTopicLineEdit"_s);
    QVERIFY(mTopicLineEdit);

    auto mUsers = w.findChild<AddUsersWidget *>(u"mUsers"_s);
    QVERIFY(mUsers);

    auto mReadOnly = w.findChild<QCheckBox *>(u"mReadOnly"_s);
    QVERIFY(mReadOnly);
    QVERIFY(!mReadOnly->toolTip().isEmpty());
    auto mBroadcast = w.findChild<QCheckBox *>(u"mBroadcast"_s);
    QVERIFY(mBroadcast);
    QVERIFY(!mBroadcast->toolTip().isEmpty());
    auto mPrivate = w.findChild<QCheckBox *>(u"mPrivate"_s);
    QVERIFY(mPrivate);
    QVERIFY(!mPrivate->toolTip().isEmpty());

    auto mEncryptedRoom = w.findChild<QCheckBox *>(u"mEncryptedRoom"_s);
    QVERIFY(mEncryptedRoom);
    QVERIFY(!mEncryptedRoom->toolTip().isEmpty());

    auto mFederated = w.findChild<QCheckBox *>(u"mFederated"_s);
    QVERIFY(mFederated);
}

#include "moc_createnewchannelwidgettest.cpp"
