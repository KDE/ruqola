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

#include "createnewchannelwidgettest.h"
#include "dialogs/channelnamevalidlineedit.h"
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
    auto mChannelName = w.findChild<ChannelNameValidLineEdit *>(QStringLiteral("mChannelName"));
    QVERIFY(mChannelName);

    auto mTopicLineEdit = w.findChild<QLineEdit *>(QStringLiteral("mTopicLineEdit"));
    QVERIFY(mTopicLineEdit);

    auto mUsers = w.findChild<AddUsersWidget *>(QStringLiteral("mUsers"));
    QVERIFY(mUsers);

    auto mReadOnly = w.findChild<QCheckBox *>(QStringLiteral("mReadOnly"));
    QVERIFY(mReadOnly);
    auto mBroadcast = w.findChild<QCheckBox *>(QStringLiteral("mBroadcast"));
    QVERIFY(mBroadcast);
    auto mPrivate = w.findChild<QCheckBox *>(QStringLiteral("mPrivate"));
    QVERIFY(mPrivate);
    auto mEncryptedRoom = w.findChild<QCheckBox *>(QStringLiteral("mEncryptedRoom"));
    QVERIFY(mEncryptedRoom);
}
