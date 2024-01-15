/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageurltest.h"
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
}

void MessageUrlTest::shouldSerializeData()
{
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setPageTitle(QStringLiteral("foo2"));
        input.setDescription(QStringLiteral("foo3"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setPageTitle(QStringLiteral("foo2"));
        input.setDescription(QStringLiteral("foo3"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        input.setImageUrl(QStringLiteral("foo4"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        input.setImageUrl(QStringLiteral("foo4"));
        input.setAuthorName(QStringLiteral("foo5"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        input.setImageUrl(QStringLiteral("foo4"));
        input.setAuthorName(QStringLiteral("foo5"));
        input.setAuthorUrl(QStringLiteral("foo6"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        input.setImageUrl(QStringLiteral("foo4"));
        input.setAuthorName(QStringLiteral("foo5"));
        input.setAuthorUrl(QStringLiteral("foo6"));
        input.setSiteUrl(QStringLiteral("foo7"));
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        input.setImageUrl(QStringLiteral("foo4"));
        input.setAuthorName(QStringLiteral("foo5"));
        input.setAuthorUrl(QStringLiteral("foo6"));
        input.setSiteUrl(QStringLiteral("foo7"));
        input.setImageHeight(8);
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
    {
        MessageUrl input;
        input.setUrl(QStringLiteral("foo1"));
        input.setDescription(QStringLiteral("foo3"));
        input.setImageUrl(QStringLiteral("foo4"));
        input.setAuthorName(QStringLiteral("foo5"));
        input.setAuthorUrl(QStringLiteral("foo6"));
        input.setSiteUrl(QStringLiteral("foo7"));
        input.setImageHeight(8);
        input.setImageWidth(32);
        const QJsonObject ba = MessageUrl::serialize(input);
        const MessageUrl output = MessageUrl::deserialize(ba);
        QCOMPARE(input, output);
    }
}

void MessageUrlTest::shouldTestPreviewUrl()
{
    MessageUrl url;
    QVERIFY(!url.hasPreviewUrl());
    url.setDescription(QStringLiteral("bla"));
    QVERIFY(url.hasPreviewUrl());
    url.setDescription(QString());
    url.setAuthorUrl(QStringLiteral("bla"));
    QVERIFY(!url.hasPreviewUrl());
    url.setImageUrl(QStringLiteral("bla1"));
    QVERIFY(url.hasPreviewUrl());
}

void MessageUrlTest::shouldGenerateHtmlDescription()
{
    QFETCH(MessageUrl, messageUrl);
    QFETCH(QString, htmlDescription);
    messageUrl.generateHtmlDescription();
    QCOMPARE(messageUrl.htmlDescription(), htmlDescription);

#if 0
    void MessageUrl::generateHtmlDescription()
    {
        mHtmlDescription.clear();
        if (!mPageTitle.isEmpty()) {
            mHtmlDescription = QStringLiteral("[%1](%2)").arg(mPageTitle, mUrl);
        }
        if (!mDescription.isEmpty()) {
            mHtmlDescription += QStringLiteral("\n%1").arg(mDescription);
        }
        if (!mSiteName.isEmpty()) {
            mHtmlDescription += QStringLiteral("\n[%1](%2)").arg(mSiteName, mSiteUrl);
        }
    }
#endif
}

void MessageUrlTest::shouldGenerateHtmlDescription_data()
{
    QTest::addColumn<MessageUrl>("messageUrl");
    QTest::addColumn<QString>("htmlDescription");
    {
        MessageUrl url;
        url.setPageTitle(QStringLiteral("Title"));
        url.setUrl(QStringLiteral("Title_url"));

        url.setDescription(QStringLiteral("Description"));

        url.setSiteName(QStringLiteral("SiteName"));
        url.setSiteUrl(QStringLiteral("SiteUrl"));

        QTest::newRow("generateHtmlDescription-test1") << url << QStringLiteral("[Title](Title_url)\nDescription\n[SiteName](SiteUrl)");
    }

    {
        MessageUrl url;
        url.setPageTitle(QStringLiteral("Title"));
        url.setUrl(QStringLiteral("Title_url"));

        url.setDescription(QStringLiteral("Description"));
        QTest::newRow("generateHtmlDescription-test2") << url << QStringLiteral("[Title](Title_url)\nDescription");
    }
    {
        MessageUrl url;
        url.setPageTitle(QStringLiteral("Title"));
        url.setUrl(QStringLiteral("Title_url"));

        QTest::newRow("generateHtmlDescription-test3") << url << QStringLiteral("[Title](Title_url)");
    }
    {
        MessageUrl url;
        QTest::newRow("generateHtmlDescription-test4") << url << QString();
    }
}

#include "moc_messageurltest.cpp"
