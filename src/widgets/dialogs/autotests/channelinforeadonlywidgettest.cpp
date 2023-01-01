/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinforeadonlywidgettest.h"
#include "dialogs/channelinforeadonlywidget.h"
#include <QFormLayout>
#include <QLabel>
#include <QTest>
QTEST_MAIN(ChannelInfoReadOnlyWidgetTest)

ChannelInfoReadOnlyWidgetTest::ChannelInfoReadOnlyWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelInfoReadOnlyWidgetTest::shouldHaveDefaultValues()
{
    ChannelInfoReadOnlyWidget w(nullptr);
    auto layoutReadOnly = w.findChild<QFormLayout *>(QStringLiteral("layoutReadOnly"));
    QVERIFY(layoutReadOnly);
    QCOMPARE(layoutReadOnly->contentsMargins(), QMargins{});

    auto mNameReadOnly = w.findChild<QLabel *>(QStringLiteral("mNameReadOnly"));
    QVERIFY(mNameReadOnly);
    QCOMPARE(mNameReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mNameReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QVERIFY(mNameReadOnly->openExternalLinks());

    auto mCommentReadOnly = w.findChild<QLabel *>(QStringLiteral("mCommentReadOnly"));
    QVERIFY(mCommentReadOnly);
    QCOMPARE(mCommentReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mCommentReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QVERIFY(mCommentReadOnly->openExternalLinks());

    auto mAnnouncementReadOnly = w.findChild<QLabel *>(QStringLiteral("mAnnouncementReadOnly"));
    QVERIFY(mAnnouncementReadOnly);
    QCOMPARE(mAnnouncementReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mAnnouncementReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QVERIFY(mAnnouncementReadOnly->openExternalLinks());

    auto mDescriptionReadOnly = w.findChild<QLabel *>(QStringLiteral("mDescriptionReadOnly"));
    QVERIFY(mDescriptionReadOnly);
    QCOMPARE(mDescriptionReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mDescriptionReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QVERIFY(mDescriptionReadOnly->openExternalLinks());
}
