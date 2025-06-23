/*
   SPDX-FileCopyrightText: 2017-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageurltest.h"
using namespace Qt::Literals::StringLiterals;

#include "messages/messageurl.h"
#include <QJsonObject>
#include <QTest>
QTEST_GUILESS_MAIN(MessageUrlTest)

MessageUrlTest::MessageUrlTest(QObject *parent)
    : QObject(parent)
{
}

void MessageUrlTest::shouldHaveDefaultValue()
{
    MessageUrl url;
    QVERIFY(url.url().isEmpty());
    QVERIFY(url.pageTitle().isEmpty());
    QVERIFY(url.description().isEmpty());
    QVERIFY(url.isEmpty());
    QVERIFY(url.imageUrl().isEmpty());
    QVERIFY(url.authorName().isEmpty());
    QVERIFY(url.authorUrl().isEmpty());
    QVERIFY(url.siteUrl().isEmpty());
    QVERIFY(url.urlId().isEmpty());
    QCOMPARE(url.imageHeight(), -1);
    QCOMPARE(url.imageWidth(), -1);
    QVERIFY(url.showPreview());
    QVERIFY(!url.hasPreviewUrl());
    QVERIFY(!url.hasHtmlDescription());
    QCOMPARE(url.contentType(), MessageUrl::ContentType::None);
}

void MessageUrlTest::shouldSerializeData()
{
    {
        MessageUrl input;
        input.setUrl(u"foo1"_s);
        input.setPageTitle(u"foo2"_s);
        input.setDescription(u"foo3"_s);
        input.generateMessageUrlInfo();
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setPageTitle(u"foo2"_s);
        input.setDescription(u"foo3"_s);
        input.generateMessageUrlInfo();
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(u"foo1"_s);
        input.setDescription(u"foo3"_s);
        input.generateMessageUrlInfo();
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(u"foo1"_s);
        input.setDescription(u"foo3"_s);
        input.setImageUrl(u"foo4"_s);
        input.generateMessageUrlInfo();
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(u"foo1"_s);
        input.setDescription(u"foo3"_s);
        input.setImageUrl(u"foo4"_s);
        input.setAuthorName(u"foo5"_s);
        input.generateMessageUrlInfo();
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(u"foo1"_s);
        input.setDescription(u"foo3"_s);
        input.setImageUrl(u"foo4"_s);
        input.setAuthorName(u"foo5"_s);
        input.setAuthorUrl(u"foo6"_s);
        input.generateMessageUrlInfo();
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(u"foo1"_s);
        input.setDescription(u"foo3"_s);
        input.setImageUrl(u"foo4"_s);
        input.setAuthorName(u"foo5"_s);
        input.setAuthorUrl(u"foo6"_s);
        input.setSiteUrl(u"foo7"_s);
        input.generateMessageUrlInfo();
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(u"foo1"_s);
        input.setDescription(u"foo3"_s);
        input.setImageUrl(u"foo4"_s);
        input.setAuthorName(u"foo5"_s);
        input.setAuthorUrl(u"foo6"_s);
        input.setSiteUrl(u"foo7"_s);
        input.setImageHeight(8);
        input.generateMessageUrlInfo();
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(u"foo1"_s);
        input.setDescription(u"foo3"_s);
        input.setImageUrl(u"foo4"_s);
        input.setAuthorName(u"foo5"_s);
        input.setAuthorUrl(u"foo6"_s);
        input.setSiteUrl(u"foo7"_s);
        input.setImageHeight(8);
        input.setImageWidth(32);
        input.generateMessageUrlInfo();
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
}

void MessageUrlTest::shouldTestPreviewUrl()
{
    MessageUrl url;
    QVERIFY(!url.hasPreviewUrl());
    url.setDescription(u"bla"_s);
    QVERIFY(url.hasPreviewUrl());
    url.setDescription(QString());
    url.setAuthorUrl(u"bla"_s);
    QVERIFY(!url.hasPreviewUrl());
    url.setImageUrl(u"bla1"_s);
    QVERIFY(url.hasPreviewUrl());
}

void MessageUrlTest::shouldGenerateHtmlDescription()
{
    QFETCH(MessageUrl, messageUrl);
    QFETCH(QString, htmlDescription);
    messageUrl.generateMessageUrlInfo();
    // qDebug() << " messageUrl.htmlDescription()" << messageUrl.htmlDescription();
    QCOMPARE(messageUrl.htmlDescription(), htmlDescription);
}

void MessageUrlTest::shouldGenerateHtmlDescription_data()
{
    QTest::addColumn<MessageUrl>("messageUrl");
    QTest::addColumn<QString>("htmlDescription");
    {
        MessageUrl url;
        url.setPageTitle(u"Title"_s);
        url.setUrl(u"Title_url"_s);

        url.setDescription(u"Description"_s);

        url.setSiteName(u"SiteName"_s);
        url.setSiteUrl(u"SiteUrl"_s);

        QTest::newRow("generateHtmlDescription-test1") << url << u"[Title](Title_url)\nDescription\n[SiteName](SiteUrl)"_s;
    }

    {
        MessageUrl url;
        url.setPageTitle(u"Title"_s);
        url.setUrl(u"Title_url"_s);

        url.setDescription(u"Description"_s);
        QTest::newRow("generateHtmlDescription-test2") << url << u"[Title](Title_url)\nDescription"_s;
    }
    {
        MessageUrl url;
        url.setPageTitle(u"Title"_s);
        url.setUrl(u"Title_url"_s);

        QTest::newRow("generateHtmlDescription-test3") << url << u"[Title](Title_url)"_s;
    }
    {
        MessageUrl url;
        QTest::newRow("generateHtmlDescription-test4") << url << QString();
    }
    {
        MessageUrl url;
        url.setPageTitle(QStringLiteral(
            " Shan Hadden Fanpage on Instagram: \"The Iconic video that started it all\n. \n. \n. \n#shanhadden #queenshanhadden #egirl #minecraft\""));
        url.setUrl(u"https://www.instagram.com/p/C0vwctGuxnI/"_s);

        url.setDescription(
            QStringLiteral("19K likes, 66 comments - queenshanfan on December 12, 2023: \"The Iconic video that started it all\n. \n. \n. \n#shanhadden "
                           "#queenshanhadden #egirl #minecraft\""));

        url.setSiteName(u"Instagram"_s);
        url.setSiteUrl(u"https://www.instagram.com/reel/C0vwctGuxnI/"_s);

        QTest::newRow("generateHtmlDescription-test5")
            << url
            << QStringLiteral(
                   "[ Shan Hadden Fanpage on Instagram: \"The Iconic video that started it all. . . #shanhadden #queenshanhadden #egirl "
                   "#minecraft\"](https://www.instagram.com/p/C0vwctGuxnI/)\n19K likes, 66 comments - queenshanfan on December 12, 2023: \"The Iconic video "
                   "that started it all. . . #shanhadden #queenshanhadden #egirl #minecraft\"\n[Instagram](https://www.instagram.com/reel/C0vwctGuxnI/)");
    }
    {
        MessageUrl url;
        url.setPageTitle(u"ZED 2 - AI Stereo Camera | Stereolabs"_s);
        url.setUrl(u"https://www.stereolabs.com/products/zed-2"_s);

        url.setDescription(
            QStringLiteral("The ZED 2 family is a next-generation series of USB 3.1 stereo cameras that seamlessly integrate advanced depth sensing with AI "
                           "capabilities. This combination empowers you to develop cutting-edge spatial intelligence applications"));

        url.setSiteName(QString());
        url.setSiteUrl(u"https://www.stereolabs.com/products/zed-"_s);

        QTest::newRow("generateHtmlDescription-test6")
            << url
            << QStringLiteral(
                   "[ZED 2 - AI Stereo Camera | Stereolabs](https://www.stereolabs.com/products/zed-2)\nThe ZED 2 family is a next-generation series of USB "
                   "3.1 stereo cameras that seamlessly integrate advanced depth sensing with AI capabilities. This combination empowers you to develop "
                   "cutting-edge spatial intelligence applications");
    }
}

void MessageUrlTest::shouldGenerateBuildImageUrl()
{
    QFETCH(MessageUrl, messageUrl);
    QFETCH(QString, buildImageUrl);
    QCOMPARE(messageUrl.buildImageUrl(), buildImageUrl);
}

void MessageUrlTest::shouldGenerateBuildImageUrl_data()
{
    QTest::addColumn<MessageUrl>("messageUrl");
    QTest::addColumn<QString>("buildImageUrl");
    {
        MessageUrl url;
        url.setUrl(u"http://bla"_s);
        url.setImageUrl(u"/foo/bla.png"_s);
        url.generateMessageUrlInfo();

        QTest::newRow("test1") << url << u"http://bla/foo/bla.png"_s;
    }
    {
        MessageUrl url;
        url.setUrl(u"http://bla"_s);
        url.setImageUrl(u"http://www.kde.org/foo/bla.png"_s);
        url.generateMessageUrlInfo();

        QTest::newRow("test2") << url << u"http://www.kde.org/foo/bla.png"_s;
    }
    {
        MessageUrl url;
        url.generateMessageUrlInfo();

        QTest::newRow("test3") << url << QString();
    }
}

void MessageUrlTest::shouldContentTypeConvert()
{
    QVERIFY(MessageUrl::contentTypeEnumToString(MessageUrl::ContentType::None).isEmpty());
    QCOMPARE(MessageUrl::contentTypeEnumToString(MessageUrl::ContentType::Image), u"image"_s);
    QCOMPARE(MessageUrl::contentTypeEnumToString(MessageUrl::ContentType::Audio), u"audio"_s);
    QCOMPARE(MessageUrl::contentTypeEnumToString(MessageUrl::ContentType::Video), u"video"_s);
    QCOMPARE(MessageUrl::contentTypeEnumToString(MessageUrl::ContentType::ImageAnimated), u"image_animated"_s);

    QCOMPARE(MessageUrl::stringToContentTypeEnum(u"foo"_s), MessageUrl::ContentType::None);
    QCOMPARE(MessageUrl::stringToContentTypeEnum(u"image"_s), MessageUrl::ContentType::Image);
    QCOMPARE(MessageUrl::stringToContentTypeEnum(u"audio"_s), MessageUrl::ContentType::Audio);
    QCOMPARE(MessageUrl::stringToContentTypeEnum(u"video"_s), MessageUrl::ContentType::Video);
    QCOMPARE(MessageUrl::stringToContentTypeEnum(u"image_animated"_s), MessageUrl::ContentType::ImageAnimated);

    MessageUrl m;
    QCOMPARE(m.parseHeaderContentType(u"foo"_s), MessageUrl::ContentType::None);
    QCOMPARE(m.parseHeaderContentType(u"image/foo"_s), MessageUrl::ContentType::Image);
    QCOMPARE(m.parseHeaderContentType(u"audio/bla"_s), MessageUrl::ContentType::Audio);
    QCOMPARE(m.parseHeaderContentType(u"video/foo"_s), MessageUrl::ContentType::Video);
    QCOMPARE(m.parseHeaderContentType(u"image/gif"_s), MessageUrl::ContentType::ImageAnimated);
    QCOMPARE(m.parseHeaderContentType(u"image"_s), MessageUrl::ContentType::None);
}

#include "moc_messageurltest.cpp"
