/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersforroommodeltest.h"
#include "model/usersforroommodel.h"
#include "test_model_helpers.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(UsersForRoomModelTest)

UsersForRoomModelTest::UsersForRoomModelTest(QObject *parent)
    : QObject(parent)
{
}

void UsersForRoomModelTest::shouldHaveDefaultValue()
{
    UsersForRoomModel w;
    QSignalSpy rowInsertedSpy(&w, &UsersForRoomModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &UsersForRoomModel::rowsAboutToBeInserted);
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
}

void UsersForRoomModelTest::shouldAddValues()
{
    UsersForRoomModel w;
    QVector<User> users;
    QSignalSpy rowInsertedSpy(&w, &UsersForRoomModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &UsersForRoomModel::rowsAboutToBeInserted);
    for (int i = 0; i < 10; i++) {
        User user;
        user.setName(QStringLiteral("name%1").arg(i));
        user.setStatus(User::PresenceStatus::PresenceAway);
        user.setUserId(QStringLiteral("userId%1").arg(i));
        user.setUserName(QStringLiteral("username%1").arg(i));
        users.append(std::move(user));
    }
    w.setUsers(users);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,9"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,9"));
    rowABTInserted.clear();
    rowInsertedSpy.clear();

    users.clear();

    for (int i = 0; i < 3; ++i) {
        User user;
        user.setName(QStringLiteral("name%1").arg(i));
        user.setStatus(User::PresenceStatus::PresenceOffline);
        user.setUserId(QStringLiteral("userId%1").arg(i));
        user.setUserName(QStringLiteral("username%1").arg(i));
        users.append(std::move(user));
    }
    w.clear();
    w.setUsers(users);
    QCOMPARE(w.rowCount(), 3);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QStringLiteral("0,2"));
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QStringLiteral("0,2"));

    rowABTInserted.clear();
    rowInsertedSpy.clear();

    users.clear();
    w.clear();
    w.setUsers(users);
    QCOMPARE(w.rowCount(), 0);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), QString());
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), QString());
}

void UsersForRoomModelTest::shouldVerifyData()
{
    UsersForRoomModel w;
    QVector<User> users;
    for (int i = 0; i < 10; i++) {
        User user;
        user.setName(QStringLiteral("name%1").arg(i));
        user.setStatus(User::PresenceStatus::PresenceOnline);
        user.setUserId(QStringLiteral("userId%1").arg(i));
        user.setUserName(QStringLiteral("username%1").arg(i));
        users.append(std::move(user));
    }
    w.setUsers(users);
    QCOMPARE(w.rowCount(), 10);

    for (int i = 0; i < 10; ++i) {
        QCOMPARE(w.data(w.index(i), UsersForRoomModel::Name).toString(), QStringLiteral("name%1").arg(i));
        QCOMPARE(w.data(w.index(i), UsersForRoomModel::IconStatus).toString(), QStringLiteral("user-online"));
        QCOMPARE(w.data(w.index(i), UsersForRoomModel::UserId).toString(), QStringLiteral("userId%1").arg(i));
        QCOMPARE(w.data(w.index(i), UsersForRoomModel::UserName).toString(), QStringLiteral("username%1").arg(i));
    }
}

void UsersForRoomModelTest::shouldParseUsers_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("numberOfUsers");
    QTest::addColumn<int>("numberOfSignal");
    QTest::addColumn<bool>("restApi");
    QTest::newRow("oneuser") << QStringLiteral("oneuser.json") << 1 << 1 << false;
    QTest::newRow("severalusers") << QStringLiteral("severalusers.json") << 14 << 1 << false;
    QTest::newRow("severalusers1-restapi") << QStringLiteral("severalusers1-restapi.json") << 2 << 1 << true;
}

QJsonObject loadFile(const QString &file)
{
    const QString originalJsonFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1String("/usersforroom/") + file;
    QFile f(originalJsonFile);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << " Unable to load file " << file;
        return {};
    }
    const QByteArray content = f.readAll();
    f.close();
    const QJsonDocument doc = QJsonDocument::fromJson(content);
    const QJsonObject root = doc.object();
    return root;
}

void UsersForRoomModelTest::shouldParseUsers()
{
    QFETCH(QString, filename);
    QFETCH(int, numberOfUsers);
    QFETCH(int, numberOfSignal);
    QFETCH(bool, restApi);
    UsersForRoomModel w;
    QSignalSpy rowInsertedSpy(&w, &UsersForRoomModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &UsersForRoomModel::rowsAboutToBeInserted);
    w.parseUsersForRooms(loadFile(filename), nullptr, restApi); // We don't have userstatus model here
    QCOMPARE(rowInsertedSpy.count(), numberOfSignal);
    QCOMPARE(rowABTInserted.count(), numberOfSignal);
    QCOMPARE(w.rowCount(), numberOfUsers);
}

#include "moc_usersforroommodeltest.cpp"
