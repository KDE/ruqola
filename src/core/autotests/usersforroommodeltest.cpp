/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

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
    const QSignalSpy rowInsertedSpy(&w, &UsersForRoomModel::rowsInserted);
    const QSignalSpy rowABTInserted(&w, &UsersForRoomModel::rowsAboutToBeInserted);
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

void UsersForRoomModelTest::shouldNotAddDuplicatedUsers()
{
    // setUsers() is incremental (one call per loadMoreUsers page), so a user already
    // in the model must not be appended again, even when the server reports him with
    // a different status/roles than in the previous page.
    auto createUser = [](int i, User::PresenceStatus status) {
        User user;
        user.setName(u"name%1"_s.arg(i));
        user.setStatus(status);
        user.setUserId(u"userId%1"_s.arg(i).toLatin1());
        user.setUserName(u"username%1"_s.arg(i));
        return user;
    };

    UsersForRoomModel w;
    QSignalSpy rowInsertedSpy(&w, &UsersForRoomModel::rowsInserted);
    QSignalSpy rowABTInserted(&w, &UsersForRoomModel::rowsAboutToBeInserted);

    QList<User> firstPage;
    for (int i = 0; i < 5; ++i) {
        firstPage.append(createUser(i, User::PresenceStatus::Online));
    }
    w.setUsers(firstPage);
    QCOMPARE(w.rowCount(), 5);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,4"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,4"_s);
    rowInsertedSpy.clear();
    rowABTInserted.clear();

    // Second page overlaps on userId3/userId4 and their status changed in between.
    QList<User> secondPage;
    for (int i = 3; i < 8; ++i) {
        secondPage.append(createUser(i, User::PresenceStatus::Offline));
    }
    w.setUsers(secondPage);
    QCOMPARE(w.rowCount(), 8);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"5,7"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"5,7"_s);
    rowInsertedSpy.clear();
    rowABTInserted.clear();

    // The already known users kept their original value, they were not updated.
    for (int i = 0; i < 8; ++i) {
        QCOMPARE(w.data(w.index(i), UsersForRoomModel::UserId).toString(), u"userId%1"_s.arg(i));
    }

    // Re-sending an already fully known page must not insert anything.
    w.setUsers(secondPage);
    QCOMPARE(w.rowCount(), 8);
    QCOMPARE(rowInsertedSpy.count(), 0);
    QCOMPARE(rowABTInserted.count(), 0);

    // clear() must forget the known ids, otherwise a reload shows an empty list.
    w.clear();
    QCOMPARE(w.rowCount(), 0);
    w.setUsers(firstPage);
    QCOMPARE(w.rowCount(), 5);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowInsertedSpy), u"0,4"_s);
    QCOMPARE(TestModelHelpers::rowSpyToText(rowABTInserted), u"0,4"_s);
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
    const QSignalSpy rowInsertedSpy(&w, &UsersForRoomModel::rowsInserted);
    const QSignalSpy rowABTInserted(&w, &UsersForRoomModel::rowsAboutToBeInserted);
    w.parseUsersForRooms(loadFile(filename), nullptr, restApi, {}); // We don't have userstatus model here
    QCOMPARE(rowInsertedSpy.count(), numberOfSignal);
    QCOMPARE(rowABTInserted.count(), numberOfSignal);
    QCOMPARE(w.rowCount(), numberOfUsers);
}

#include "moc_usersforroommodeltest.cpp"
