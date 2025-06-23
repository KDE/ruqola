/*
   SPDX-FileCopyrightText: 2018-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersforroommodeltest.h"
using namespace Qt::Literals::StringLiterals;

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
    QList<User> users;
    QSignalSpy rowInsertedSpy(&w, &UsersForRoomModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &UsersForRoomModel::rowsAboutToBeInserted);
    for (int i = 0; i < 10; i++) {
        User user;
        user.setName(u"name%1"_s.arg(i));
        user.setStatus(User::PresenceStatus::Away);
        user.setUserId(u"userId%1"_s.arg(i).toLatin1());
        user.setUserName(u"username%1"_s.arg(i));
        users.append(std::move(user));
    }
    w.setUsers(users);
    QCOMPARE(w.rowCount(), 10);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,9"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,9"_s);
    rowABTInserted.clear();
    rowInsertedSpy.clear();

    users.clear();

    for (int i = 0; i < 3; ++i) {
        User user;
        user.setName(u"name%1"_s.arg(i));
        user.setStatus(User::PresenceStatus::Offline);
        user.setUserId(u"userId%1"_s.arg(i).toLatin1());
        user.setUserName(u"username%1"_s.arg(i));
        users.append(std::move(user));
    }
    w.clear();
    w.setUsers(users);
    QCOMPARE(w.rowCount(), 3);
    QCOMPARE(rowInsertedSpy.count(), 1);
    QCOMPARE(rowABTInserted.count(), 1);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,2"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,2"_s);

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
    QList<User> users;
    for (int i = 0; i < 10; i++) {
        User user;
        user.setName(u"name%1"_s.arg(i));
        user.setStatus(User::PresenceStatus::Online);
        user.setUserId(u"userId%1"_s.arg(i).toLatin1());
        user.setUserName(u"username%1"_s.arg(i));
        users.append(std::move(user));
    }
    w.setUsers(users);
    QCOMPARE(w.rowCount(), 10);

    for (int i = 0; i < 10; ++i) {
        QCOMPARE(w.data(w.index(i), UsersForRoomModel::Name).toString(), u"name%1"_s.arg(i));
        QCOMPARE(w.data(w.index(i), UsersForRoomModel::IconStatus).toString(), u"user-online"_s);
        QCOMPARE(w.data(w.index(i), UsersForRoomModel::UserId).toString(), u"userId%1"_s.arg(i));
        QCOMPARE(w.data(w.index(i), UsersForRoomModel::UserName).toString(), u"username%1"_s.arg(i));
    }
}

void UsersForRoomModelTest::shouldParseUsers_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<int>("numberOfUsers");
    QTest::addColumn<int>("numberOfSignal");
    QTest::addColumn<bool>("restApi");
    QTest::newRow("oneuser") << u"oneuser.json"_s << 1 << 1 << false;
    QTest::newRow("severalusers") << u"severalusers.json"_s << 14 << 1 << false;
    QTest::newRow("severalusers1-restapi") << u"severalusers1-restapi.json"_s << 2 << 1 << true;
}

static QJsonObject loadFile(const QString &file)
{
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/usersforroom/"_L1 + file;
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
    w.parseUsersForRooms(loadFile(filename), nullptr, restApi, {}); // We don't have userstatus model here
    QCOMPARE(rowInsertedSpy.count(), numberOfSignal);
    QCOMPARE(rowABTInserted.count(), numberOfSignal);
    QCOMPARE(w.rowCount(), numberOfUsers);
}

#include "moc_usersforroommodeltest.cpp"
