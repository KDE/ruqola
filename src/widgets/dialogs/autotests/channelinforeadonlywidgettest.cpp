/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelinforeadonlywidgettest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto layoutReadOnly = w.findChild<QFormLayout *>(u"layoutReadOnly"_s);
    QVERIFY(layoutReadOnly);
    QCOMPARE(layoutReadOnly->contentsMargins(), QMargins{});

    auto mNameReadOnly = w.findChild<QLabel *>(u"mNameReadOnly"_s);
    QVERIFY(mNameReadOnly);
    QCOMPARE(mNameReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mNameReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QVERIFY(mNameReadOnly->openExternalLinks());
#if 0 // Now it's loaded on demand
    auto mCommentReadOnly = w.findChild<QLabel *>(u"mCommentReadOnly"_s);
    QVERIFY(mCommentReadOnly);
    QCOMPARE(mCommentReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mCommentReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QVERIFY(mCommentReadOnly->openExternalLinks());

    auto mAnnouncementReadOnly = w.findChild<QLabel *>(u"mAnnouncementReadOnly"_s);
    QVERIFY(mAnnouncementReadOnly);
    QCOMPARE(mAnnouncementReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mAnnouncementReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QVERIFY(mAnnouncementReadOnly->openExternalLinks());

    auto mDescriptionReadOnly = w.findChild<QLabel *>(u"mDescriptionReadOnly"_s);
    QVERIFY(mDescriptionReadOnly);
    QCOMPARE(mDescriptionReadOnly->textFormat(), Qt::RichText);
    QCOMPARE(mDescriptionReadOnly->textInteractionFlags(), Qt::TextBrowserInteraction);
    QVERIFY(mDescriptionReadOnly->openExternalLinks());
#endif
}

#include "moc_channelinforeadonlywidgettest.cpp"
