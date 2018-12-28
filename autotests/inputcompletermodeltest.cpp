/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "inputcompletermodeltest.h"
#include "model/inputcompletermodel.h"
#include <QTest>
#include "test_model_helpers.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>

QTEST_MAIN(InputCompleterModelTest)

InputCompleterModelTest::InputCompleterModelTest(QObject *parent)
    : QObject(parent)
{
}

void InputCompleterModelTest::shouldHaveDefaultValue()
{
    InputCompleterModel w;
    QCOMPARE(w.rowCount(), 0);

    QHash<int, QByteArray> roles;
    roles[InputCompleterModel::DisplayName] = QByteArrayLiteral("displayname");
    roles[InputCompleterModel::CompleterName] = QByteArrayLiteral("completername");
    roles[InputCompleterModel::IconName] = QByteArrayLiteral("iconname");
    roles[InputCompleterModel::ChannelType] = QByteArrayLiteral("channeltype");
    QCOMPARE(w.roleNames(), roles);
}

void InputCompleterModelTest::shouldAssignValues()
{
    InputCompleterModel w;
    QSignalSpy rowInsertedSpy(&w, &InputCompleterModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &InputCompleterModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &InputCompleterModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &InputCompleterModel::rowsAboutToBeRemoved);

    QVector<Channel> channelList;
    for (int i = 0; i < 10; ++i) {
        Channel c;
        c.setRoomId(QStringLiteral("roomid%1").arg(i));
        c.setRoomName(QStringLiteral("roomname%1").arg(i));
        c.setRoomType(QStringLiteral("online"));
        channelList.append(c);
    }
    w.setChannels(channelList);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,9"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,9"));

    //add Empty list
    channelList.clear();
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.setChannels(channelList);

    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,9"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,9"));

    //Add same element
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.setChannels(channelList);

    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QString());

    //Test add same number of element.
    channelList.clear();
    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    for (int i = 0; i < 5; ++i) {
        Channel c;
        c.setRoomId(QStringLiteral("roomid%1").arg(i));
        c.setRoomName(QStringLiteral("roomname%1").arg(i));
        c.setRoomType(QStringLiteral("online"));
        channelList.append(c);
    }
    w.setChannels(channelList);
    QCOMPARE(w.rowCount(), 5);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,4"));

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();
    w.setChannels(channelList);

    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,4"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,4"));
}

QJsonObject loadFile(const QString &file)
{
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QStringLiteral("/json/") + file;
    QFile f(originalJsonFile);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << " Unable to load file " << file;
        return {};
    }
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject root = doc.object();
    const QJsonObject obj = root.value(QLatin1String("result")).toObject();
    return obj;
}

void InputCompleterModelTest::shouldLoadValueFromJson()
{
    InputCompleterModel w;
    QSignalSpy rowInsertedSpy(&w, &InputCompleterModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &InputCompleterModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &InputCompleterModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &InputCompleterModel::rowsAboutToBeRemoved);

    QJsonObject obj = loadFile(QStringLiteral("channelparent.json"));
    w.parseChannels(obj);
    QCOMPARE(w.rowCount(), 8);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,7"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,7"));

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    //Test room
    QCOMPARE(w.data(w.index(2), InputCompleterModel::CompleterName).toString(), QStringLiteral("bal3"));
    QCOMPARE(w.data(w.index(2), InputCompleterModel::ChannelType).value<Channel::ChannelType>(), Channel::ChannelType::Room);
    QCOMPARE(w.data(w.index(2), InputCompleterModel::DisplayName).toString(), QStringLiteral("bal3"));

    //Test user
    QCOMPARE(w.data(w.index(6), InputCompleterModel::CompleterName).toString(), QStringLiteral("bla.foo4"));
    QCOMPARE(w.data(w.index(6), InputCompleterModel::ChannelType).value<Channel::ChannelType>(), Channel::ChannelType::PrivateChannel);
    //We use for user a channelid == channel name as we use it for opening direct channel
    QCOMPARE(w.data(w.index(6), InputCompleterModel::DisplayName).toString(), QStringLiteral("bla.foo4 (foo4)"));

    //Test without name/username !

    obj = loadFile(QStringLiteral("channelparentempty.json"));
    w.parseChannels(obj);
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,7"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,7"));
}

void InputCompleterModelTest::shouldClearModel()
{
    InputCompleterModel w;
    QSignalSpy rowInsertedSpy(&w, &InputCompleterModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &InputCompleterModel::rowsAboutToBeInserted);
    QSignalSpy rowRemovedSpy(&w, &InputCompleterModel::rowsRemoved);
    QSignalSpy rowABTRemoved(&w, &InputCompleterModel::rowsAboutToBeRemoved);

    QJsonObject obj = loadFile(QStringLiteral("channelparent.json"));
    w.parseChannels(obj);
    QCOMPARE(w.rowCount(), 8);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(rowRemovedSpy.count(), 0);
    QCOMPARE(rowABTRemoved.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,7"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,7"));

    rowInsertedSpy.clear();
    rowABTInserted.clear();
    rowRemovedSpy.clear();
    rowABTRemoved.clear();

    w.clear();
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(rowRemovedSpy.count(), 1);
    QCOMPARE(rowABTRemoved.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowRemovedSpy), QStringLiteral("0,7"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTRemoved), QStringLiteral("0,7"));
}
