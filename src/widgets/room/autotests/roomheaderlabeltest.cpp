/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomheaderlabeltest.h"
#include "room/roomheaderlabel.h"
#include <QTest>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextFragment>
QTEST_MAIN(RoomHeaderLabelTest)
using namespace Qt::Literals::StringLiterals;
RoomHeaderLabelTest::RoomHeaderLabelTest(QObject *parent)
    : QObject(parent)
{
}

void RoomHeaderLabelTest::shouldHaveDefaultValues()
{
    RoomHeaderLabel w;
    QVERIFY(w.fullText().isEmpty());
    // TODO
}

void RoomHeaderLabelTest::shouldKeepShowMoreAtNormalLineSpacing()
{
    RoomHeaderLabel w;
    w.resize(1000, 200);
    w.setRoomTopic(u"<qt><p>Main topic</p>\n<p>Second line</p>\n<p>Third line</p>\n<p>Fourth line</p></qt>"_s);

    QVERIFY(w.text().contains(u"<br />"_s));
    QVERIFY(w.text().contains(u"href=\"showmoretext\""_s));

    Q_EMIT w.linkActivated(u"showmoretext"_s);
    QVERIFY(w.text().contains(u"<br />"_s));
    QVERIFY(w.text().contains(u"href=\"showlesstext\""_s));
}

void RoomHeaderLabelTest::shouldCollapseWrappedSingleParagraph()
{
    RoomHeaderLabel w;
    w.resize(240, 200);
    w.setRoomTopic(
        u"This is a long single-paragraph topic that wraps over many visual lines at the label's actual width and must remain collapsed by default."_s);

    QVERIFY(w.text().contains(u"href=\"showmoretext\""_s));
}

void RoomHeaderLabelTest::shouldUseFullWidthBeforeShowMore()
{
    RoomHeaderLabel w;
    w.resize(420, 200);

    const int availableWidth = w.width() - 10;
    const int showMoreWidth = w.fontMetrics().horizontalAdvance(u"(Show More Info…)"_s);
    QString firstLine;
    const QString word = u"topic "_s;
    while (w.fontMetrics().horizontalAdvance(firstLine + word) < availableWidth - 10) {
        firstLine += word;
    }
    firstLine = firstLine.trimmed();
    QVERIFY(w.fontMetrics().horizontalAdvance(firstLine) > availableWidth - showMoreWidth);

    w.setRoomTopic(firstLine + u" tail tail tail tail tail tail<br>Second line<br>Third line<br>Fourth line"_s);

    QTextDocument document;
    document.setHtml(w.text());
    QVERIFY(document.toPlainText().section(u'\n', 0, 0).startsWith(firstLine));
}

void RoomHeaderLabelTest::shouldPreserveRichTextWhenCollapsing()
{
    RoomHeaderLabel w;
    w.resize(300, 200);
    const QString originalUrl = u"https://example.com/"_s + QString(300, u'a');
    w.setRoomTopic(
        u"<p>See <a href=\"%1\">the link text that continues across the first visual line and beyond it</a> followed by more text.</p>"
        "<p>Second paragraph</p><p>Third paragraph</p><p>Fourth paragraph</p>"_s.arg(originalUrl));

    QStringList anchors;
    QTextDocument document;
    document.setHtml(w.text());
    for (QTextBlock block = document.begin(); block.isValid(); block = block.next()) {
        for (QTextBlock::iterator it = block.begin(); !it.atEnd(); ++it) {
            const QTextFragment fragment = it.fragment();
            if (fragment.isValid() && fragment.charFormat().isAnchor()) {
                anchors.append(fragment.charFormat().anchorHref());
            }
        }
    }
    QVERIFY(anchors.contains(originalUrl));
    QVERIFY(anchors.contains(u"showmoretext"_s));
}

#include "moc_roomheaderlabeltest.cpp"
