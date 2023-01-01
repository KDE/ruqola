/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomheaderlabel.h"

#include <KLocalizedString>
#include <QDesktopServices>
#include <QTextDocument>

RoomHeaderLabel::RoomHeaderLabel(QWidget *parent)
    : QLabel(parent)
{
    setWordWrap(true);
    setTextInteractionFlags(Qt::TextBrowserInteraction);
    setTextFormat(Qt::RichText);
    setVisible(false);
    connect(this, &QLabel::linkActivated, this, &RoomHeaderLabel::slotMoreInfo);
}

RoomHeaderLabel::~RoomHeaderLabel() = default;

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
    QLabel::setText(QLatin1String("<qt>") + text + QLatin1String("</qt>"));
    if (mFullText != text && !mExpandTopic) {
        setToolTip(mFullText);
    }
}

void RoomHeaderLabel::slotMoreInfo(const QString &content)
{
    if (content == QLatin1String("showmoretext")) {
        mExpandTopic = true;
        updateSqueezedText();
    } else if (content == QLatin1String("showlesstext")) {
        mExpandTopic = false;
        updateSqueezedText();
    } else {
        QDesktopServices::openUrl(QUrl(content));
    }
}

QString RoomHeaderLabel::rPixelSqueeze(const QString &text, int maxPixels) const
{
    const auto tSize = textSize(text);
    int tHeight = tSize.height();
    int tw = tSize.width();
    QString tmp = text;
    const QString showMoreText = i18n("(Show More Info...)");
    if (tHeight > (2 * fontMetrics().ascent() + fontMetrics().descent())) {
        if (!mExpandTopic) {
            if (tw > maxPixels) {
                int em = fontMetrics().maxWidth();
                maxPixels -= fontMetrics().horizontalAdvance(showMoreText);

                // On some MacOS system, maxWidth may return 0
                if (em == 0) {
                    for (QChar c : text) {
                        em = qMax(em, fontMetrics().horizontalAdvance(c));
                    }
                }
                while ((tw > maxPixels) && !tmp.isEmpty()) {
                    const int len = tmp.length();
                    int delta = (em == 0) ? 0 : (tw - maxPixels) / em;
                    delta = qBound(1, delta, len);

                    tmp.remove(len - delta, delta);
                    tw = textSize(tmp).width();
                }
            } else {
                tmp = tmp.split(QStringLiteral("\n")).at(0);
            }
            if (!tmp.endsWith(QLatin1Char('\n'))) {
                tmp.append(QLatin1Char('\n'));
            }
            return tmp.append(QStringLiteral("<a href=\"showmoretext\"> %1</a>").arg(showMoreText));
        } else {
            if (!tmp.endsWith(QLatin1Char('\n'))) {
                tmp.append(QLatin1Char('\n'));
            }
            return tmp.append(QStringLiteral("<a href=\"showlesstext\"> %1</a>").arg(i18n("(Show Less Info...)")));
        }
    }
    return text;
}

QSize RoomHeaderLabel::textSize(const QString &text) const
{
    QTextDocument document;
    document.setDefaultFont(font());
    document.setHtml(QLatin1String("<qt>") + text + QLatin1String("</qt>"));

    return document.size().toSize();
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
    if (!mTopic.isEmpty()) {
        mFullText = mTopic;
    }
    if (!mAnnouncement.isEmpty()) {
        if (!mFullText.isEmpty()) {
            mFullText += QLatin1Char('\n');
        }
        mFullText += mAnnouncement;
    }
    updateSqueezedText();
}
