/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomheaderlabel.h"

#include "ruqolautils.h"
#include <KLocalizedString>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextLayout>
using namespace Qt::Literals::StringLiterals;

namespace
{
int lineCount(const QTextDocument &document)
{
    int count = 0;
    for (QTextBlock block = document.begin(); block.isValid(); block = block.next()) {
        count += block.layout()->lineCount();
    }
    return count;
}

int firstVisualLineEnd(const QTextDocument &document)
{
    for (QTextBlock block = document.begin(); block.isValid(); block = block.next()) {
        const QTextLayout *layout = block.layout();
        if (layout->lineCount() > 0) {
            const QTextLine line = layout->lineAt(0);
            int end = block.position() + line.textStart() + line.textLength();
            while (end > block.position() && document.characterAt(end - 1).isSpace()) {
                --end;
            }
            return end;
        }
    }
    return 0;
}

QString appendInfoLink(QTextDocument &document, const QString &href, const QString &linkText)
{
    QTextCursor cursor(&document);
    cursor.movePosition(QTextCursor::End);
    cursor.insertHtml(u"<br><a href=\"%1\"> %2</a>"_s.arg(href, linkText.toHtmlEscaped()));
    return document.toHtml();
}
}

RoomHeaderLabel::RoomHeaderLabel(QWidget *parent)
    : QLabel(parent)
{
    setWordWrap(true);
    setTextInteractionFlags(Qt::TextBrowserInteraction);
    setTextFormat(Qt::RichText);
    setVisible(false);
    connect(this, &QLabel::linkActivated, this, &RoomHeaderLabel::slotMoreInfo);
    connect(this, &QLabel::linkHovered, this, [this](const QString &url) {
        if (url != "showlesstext"_L1 && url != "showmoretext"_L1 && url != "add_topic"_L1) {
            setToolTip(url);
        }
    });
}

RoomHeaderLabel::~RoomHeaderLabel() = default;

void RoomHeaderLabel::setIsOwner(bool isOwner)
{
    mIsOwner = isOwner;
    updateHeaderText();
}

void RoomHeaderLabel::resizeEvent(QResizeEvent *ev)
{
    QLabel::resizeEvent(ev);
    updateSqueezedText();
}

void RoomHeaderLabel::updateSqueezedText()
{
    setToolTip(QString());

    if (mFullText.isEmpty()) {
        QLabel::setText(QString());
        setVisible(false);
        return;
    }
    setVisible(true);
    const QString text = rPixelSqueeze(mFullText, width() - 10);
    QLabel::setText(text);
    if (mFullText != text && !mExpandTopic) {
        setToolTip(mFullText);
    }
}

void RoomHeaderLabel::slotMoreInfo(const QString &content)
{
    if (content == "showmoretext"_L1) {
        mExpandTopic = true;
        updateSqueezedText();
    } else if (content == "showlesstext"_L1) {
        mExpandTopic = false;
        updateSqueezedText();
    } else if (content == "add_topic"_L1) {
        Q_EMIT configureTopic();
    } else {
        RuqolaUtils::self()->openUrl(QUrl(content));
    }
}

QString RoomHeaderLabel::rPixelSqueeze(const QString &text, int maxPixels) const
{
    QTextDocument document;
    document.setDefaultFont(font());
    document.setDocumentMargin(0);
    document.setHtml(text);
    document.setTextWidth(qMax(1, maxPixels));
    document.size(); // Force the width-constrained layout before inspecting its lines.

    if (lineCount(document) <= 3) {
        return text;
    }

    if (mExpandTopic) {
        return appendInfoLink(document, u"showlesstext"_s, i18n("(Show Less Info…)"));
    }

    QTextCursor selection(&document);
    selection.setPosition(0);
    selection.setPosition(firstVisualLineEnd(document), QTextCursor::KeepAnchor);

    QTextDocument collapsedDocument;
    collapsedDocument.setDefaultFont(font());
    collapsedDocument.setDocumentMargin(0);
    QTextCursor collapsedCursor(&collapsedDocument);
    collapsedCursor.insertFragment(selection.selection());
    return appendInfoLink(collapsedDocument, u"showmoretext"_s, i18n("(Show More Info…)"));
}

const QString &RoomHeaderLabel::fullText() const
{
    return mFullText;
}

void RoomHeaderLabel::setRoomAnnouncement(const QString &announcement)
{
    mAnnouncement = announcement;
    updateHeaderText();
}

void RoomHeaderLabel::setRoomTopic(const QString &topic)
{
    mTopic = topic;
    updateHeaderText();
}

void RoomHeaderLabel::updateHeaderText()
{
    mFullText.clear();
    if (mTopic.isEmpty()) {
        if (mIsOwner) {
            mFullText = u"<a href=\"add_topic\">%1</a>"_s.arg(i18n("Add Topic"));
        }
    } else {
        mFullText = mTopic;
    }
    if (!mAnnouncement.isEmpty()) {
        if (!mFullText.isEmpty()) {
            mFullText += u'\n';
        }
        mFullText += mAnnouncement;
    }
    updateSqueezedText();
}

#include "moc_roomheaderlabel.cpp"
