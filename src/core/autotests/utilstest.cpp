/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "utilstest.h"
using namespace Qt::Literals::StringLiterals;

#include "utils.h"
#include <QJsonDocument>
#include <QTest>

QTEST_GUILESS_MAIN(UtilsTest)

UtilsTest::UtilsTest(QObject *parent)
    : QObject(parent)
{
}

void UtilsTest::shouldGenerateServerUrl_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QUrl>("output");
    QTest::newRow("empty") << QString() << QUrl();
    QTest::newRow("http") << u"http://foo.kde.org/"_s << QUrl(u"ws://foo.kde.org//websocket"_s);
    QTest::newRow("http2") << u"http://foo.kde.org"_s << QUrl(u"ws://foo.kde.org/websocket"_s);
    QTest::newRow("https") << u"https://foo.kde.org"_s << QUrl(u"wss://foo.kde.org/websocket"_s);
    QTest::newRow("withoutscheme") << u"foo.kde.org"_s << QUrl(u"wss://foo.kde.org/websocket"_s);
}

void UtilsTest::shouldGenerateServerUrl()
{
    QFETCH(QString, input);
    QFETCH(QUrl, output);
    QCOMPARE(Utils::generateServerUrl(input), output);
}

void UtilsTest::shouldExtractRoomUserFromUrl_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("empty") << QString() << QString();

    QTest::newRow("extractuser") << u"ruqola:/user/foo"_s << u"foo"_s;
    QTest::newRow("extractroom") << u"ruqola:/room/foo"_s << u"foo"_s;
    QTest::newRow("extractuser2") << u"ruqola:/user2/foo"_s << u"ruqola:/user2/foo"_s;
}

void UtilsTest::shouldExtractRoomUserFromUrl()
{
    QFETCH(QString, input);
    QFETCH(QString, output);
    QCOMPARE(Utils::extractRoomUserFromUrl(input), output);
}

void UtilsTest::shouldConvertTextWithUrl_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("convertedText");
    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("onlytext") << u"foo bla bli"_s << u"foo bla bli"_s;
    QTest::newRow("test1") << u"[blo](http://www.kde.org)"_s << u"<a href='http://www.kde.org'>blo</a>"_s;
    QTest::newRow("test2") << u"[](http://www.kde.org)"_s << u"(http://www.kde.org)"_s;
    QTest::newRow("test3") << u"bla bla [blo](http://www.kde.org)"_s << u"bla bla <a href='http://www.kde.org'>blo</a>"_s;
    QTest::newRow("test4") << u"bla bla [blo](http://www.kde.org) bli [blu](http://www.kdi.org)"_s
                           << u"bla bla <a href='http://www.kde.org'>blo</a> bli <a href='http://www.kdi.org'>blu</a>"_s;
    QTest::newRow("test5") << u"bla bla [blo]"_s << u"bla bla [blo]"_s;
    QTest::newRow("test6") << u"bla bla [blo] bli"_s << u"bla bla [blo] bli"_s;
    // Test <https://www.kde.org|bla>
    QTest::newRow("[https://www.kde.org|bla]") << QStringLiteral(
        "bla [<a href=\"https://www.kde.org.|https://www.kde.org\">https://www.kde.org.|https://www.kde.org</a>]")
                                               << u"bla <a href=\"https://www.kde.org\">https://www.kde.org.</a>"_s;
#if 0
    QTest::newRow("[https://www.kde.org|bla]") << u"[https://www.kde.org|https://www.kde.org/bla]"_s
                                               << u"<a href='https://www.kde.org/bla'>https://www.kde.org</a>"_s;
    QTest::newRow("[https://www.kde.com/pages/viewpage.action?pageId=111111.|https://www.kde.com/pages/viewpage.action?pageId=111111]")
        << u"[https://www.kde.com//pages/viewpage.action?pageId=111111.|https://www.kde.com//pages/viewpage.action?pageId=111111]"_s
        << u"<a href='https://www.kde.com//pages/viewpage.action?pageId=111111'>https://www.kde.com//pages/viewpage.action?pageId=111111.</a>"_s;
    QTest::newRow("blabla [https://www.kde.org|https://www.kde.org/bla]") << u"blabla [https://www.kde.org|https://www.kde.org/bla]"_s
                                                                          << u"blabla <a href='https://www.kde.org/bla'>https://www.kde.org</a>"_s;

    QTest::newRow("blabla [https://www.kde.org|https://www.kde.org/bla] 2 ")
        << QStringLiteral(
               "ideas: [https://www.kde.com/pages/viewpage.action?pageId=11111.|https://www.kde.com/pages/viewpage.action?pageId=11111]\r\n [~vvvv] can")
        << QStringLiteral(
               "ideas: <a href='https://www.kde.com/pages/viewpage.action?pageId=11111'>https://www.kde.com/pages/viewpage.action?pageId=11111.</a>\r\n "
               "[~vvvv] can");
#endif
    // Test [foo](http://www.kde.org_!!)
    QTest::newRow("[foo](http://www.kde.org!!)") << u"[foo](http://www.kde.org!!)"_s << u"<a href='http://www.kde.org!!'>foo</a>"_s;
}

void UtilsTest::shouldConvertTextWithUrl()
{
    QFETCH(QString, text);
    QFETCH(QString, convertedText);
    QCOMPARE(Utils::convertTextWithUrl(text), convertedText);
}

void UtilsTest::shouldGenerateAvatarUrl()
{
    QFETCH(QString, serverUrl);
    QFETCH(Utils::AvatarInfo, avatarInfo);
    QFETCH(QUrl, result);
    QCOMPARE(avatarInfo.avatarUrl(serverUrl), result);
}

void UtilsTest::shouldGenerateAvatarUrl_data()
{
    QTest::addColumn<QString>("serverUrl");
    QTest::addColumn<Utils::AvatarInfo>("avatarInfo");
    QTest::addColumn<QUrl>("result");
    {
        Utils::AvatarInfo avatarInfo;
        QTest::newRow("empty") << QString() << avatarInfo << QUrl();
    }
    {
        Utils::AvatarInfo avatarInfo;
        avatarInfo.identifier = u"user1"_s;
        avatarInfo.avatarType = Utils::AvatarType::User;
        QTest::newRow("user1") << u"http://www.kde.org"_s << avatarInfo
                               << QUrl(u"http://www.kde.org/avatar/%1?format=png&size=22"_s.arg(avatarInfo.identifier));
    }
    {
        Utils::AvatarInfo avatarInfo;
        avatarInfo.identifier = u"user1"_s;
        avatarInfo.avatarType = Utils::AvatarType::User;
        avatarInfo.etag = u"etag-user-identifier"_s;
        QTest::newRow("user1-etag") << u"http://www.kde.org"_s << avatarInfo
                                    << QUrl(u"http://www.kde.org/avatar/%1?format=png&etag=%2&size=22"_s.arg(avatarInfo.identifier, avatarInfo.etag));
    }
    {
        Utils::AvatarInfo avatarInfo;
        avatarInfo.identifier = u"room1"_s;
        avatarInfo.avatarType = Utils::AvatarType::Room;
        QTest::newRow("room1") << u"http://www.kde.org"_s << avatarInfo
                               << QUrl(u"http://www.kde.org/avatar/room/%1?format=png&size=22"_s.arg(avatarInfo.identifier));
    }
    {
        Utils::AvatarInfo avatarInfo;
        avatarInfo.identifier = u"room1"_s;
        avatarInfo.avatarType = Utils::AvatarType::Room;
        avatarInfo.etag = u"etagIdentifier"_s;
        QTest::newRow("room1-etag") << u"http://www.kde.org"_s << avatarInfo
                                    << QUrl(u"http://www.kde.org/avatar/room/%1?format=png&etag=%2&size=22"_s.arg(avatarInfo.identifier, avatarInfo.etag));
    }
    {
        Utils::AvatarInfo avatarInfo;
        avatarInfo.identifier = u"room1"_s;
        avatarInfo.avatarType = Utils::AvatarType::Room;
        avatarInfo.etag = u"etagIdentifier"_s;
        QTest::newRow("room1-etag-without-protocol") << u"www.kde.org"_s << avatarInfo
                                                     << QUrl(u"https://www.kde.org/avatar/room/%1?format=png&etag=%2&size=22"_s.arg(avatarInfo.identifier,
                                                                                                                                    avatarInfo.etag));
    }
}

void UtilsTest::shouldGenerateCheckMark_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("convertedText");
    QTest::newRow("empty") << QString() << QString();
    QTest::newRow("unckecked1") << u"- [ ] vvv"_s << u":white_medium_square: vvv"_s;
    QTest::newRow("unckecked2") << u"- [ ] vvv\n- [ ] bla2"_s << u":white_medium_square: vvv\n:white_medium_square: bla2"_s;

    QTest::newRow("ckecked1") << u"- [x] vvv"_s << u":ballot_box_with_check: vvv"_s;
    QTest::newRow("ckecked2") << u"- [x] vvv\n- [x] bla2"_s << u":ballot_box_with_check: vvv\n:ballot_box_with_check: bla2"_s;

    QTest::newRow("mixted1") << u"- [x] vvv\n- [ ] bla2"_s << u":ballot_box_with_check: vvv\n:white_medium_square: bla2"_s;
}

void UtilsTest::shouldGenerateCheckMark()
{
    QFETCH(QString, text);
    QFETCH(QString, convertedText);
    QCOMPARE(Utils::convertTextWithCheckMark(text), convertedText);
}

void UtilsTest::shouldTestAvatarInfoValues()
{
    Utils::AvatarInfo info;
    QVERIFY(!info.isValid());
    QVERIFY(info.etag.isEmpty());
    QVERIFY(info.identifier.isEmpty());
    QCOMPARE(info.avatarType, Utils::AvatarType::Unknown);

    info.identifier = u"bla"_s;
    QCOMPARE(info.generateAvatarIdentifier(), u"bla"_s);
    info.etag = u"foo"_s;
    QCOMPARE(info.generateAvatarIdentifier(), u"bla-foo"_s);
}

void UtilsTest::shouldGenerateUniqueAccountName()
{
    QFETCH(QStringList, list);
    QFETCH(QString, originalAccountName);
    QFETCH(QString, newAccountName);
    QCOMPARE(Utils::createUniqueAccountName(list, originalAccountName), newAccountName);
}

void UtilsTest::shouldGenerateUniqueAccountName_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<QString>("originalAccountName");
    QTest::addColumn<QString>("newAccountName");
    QTest::newRow("empty") << QStringList() << QString() << QString();
    QTest::newRow("empty-accountline") << QStringList() << u"bla"_s << u"bla"_s;
    {
        QStringList lst;
        lst << u"foo"_s;
        lst << u"foo2"_s;
        lst << u"ku"_s;
        QTest::newRow("accountlist1") << lst << u"bla"_s << u"bla"_s;
    }
    {
        QStringList lst;
        lst << u"foo"_s;
        lst << u"foo2"_s;
        lst << u"ku"_s;
        lst << u"bla"_s;
        QTest::newRow("accountlist2") << lst << u"bla"_s << u"bla1"_s;
    }
    {
        QStringList lst;
        lst << u"foo"_s;
        lst << u"foo2"_s;
        lst << u"ku"_s;
        lst << u"bla"_s;
        lst << u"bla1"_s;
        lst << u"bla2"_s;
        lst << u"bla4"_s;
        QTest::newRow("accountlist3") << lst << u"bla"_s << u"bla3"_s;
    }
    {
        QStringList lst;
        lst << u"foo"_s;
        lst << u"foo2"_s;
        lst << u"ku"_s;
        lst << u"bla"_s;
        lst << u"bla1"_s;
        lst << u"bla2"_s;
        lst << u"bla3"_s;
        QTest::newRow("accountlist4") << lst << u"bla"_s << u"bla4"_s;
    }
}

void UtilsTest::shouldTestUserActivity()
{
    QFETCH(QJsonArray, array);
    QFETCH(bool, status);
    QCOMPARE(Utils::userActivity(array), status);
}

void UtilsTest::shouldTestUserActivity_data()
{
    QTest::addColumn<QJsonArray>("array");
    QTest::addColumn<bool>("status");
    QTest::newRow("empty") << QJsonArray() << false;
    {
        const QJsonDocument doc = QJsonDocument::fromJson("[\"bla\",[\"user-typing\"],{}]");
        const QJsonArray array = doc.array();
        QTest::newRow("test1") << array << true;
    }
    {
        const QJsonDocument doc = QJsonDocument::fromJson("[\"bla\",[],{}]");
        const QJsonArray array = doc.array();
        QTest::newRow("test2") << array << false;
    }
    {
        const QJsonDocument doc = QJsonDocument::fromJson("[\"bla\",true]");
        const QJsonArray array = doc.array();
        QTest::newRow("test3") << array << true;
    }
}

#include "moc_utilstest.cpp"
