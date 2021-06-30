/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

RoomHeaderLabel::~RoomHeaderLabel()
{
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
    QLabel::setText(QLatin1String("<qt>") + text + QLatin1String("</qt>"));
    setToolTip(mFullText);
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
    const QString showMoreText = i18n("(Show More Infos...)");
    if (tHeight > (2 * fontMetrics().ascent() + fontMetrics().descent())) {
        if (!mExpandTopic) {
            if (tw > maxPixels) {
                int em = fontMetrics().maxWidth();
                maxPixels -= fontMetrics().horizontalAdvance(showMoreText);
                int len, delta;

                // On some MacOS system, maxWidth may return 0
                if (em == 0) {
                    for (QChar c : text) {
                        em = qMax(em, fontMetrics().horizontalAdvance(c));
                    }
                }
                while ((tw > maxPixels) && !tmp.isEmpty()) {
                    len = tmp.length();
                    delta = (em == 0) ? 0 : (tw - maxPixels) / em;
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
