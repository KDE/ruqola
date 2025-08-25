/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "roomheaderlabel.h"

#include "ruqolautils.h"
#include <KLocalizedString>
#include <QTextDocument>
using namespace Qt::Literals::StringLiterals;

RoomHeaderLabel::RoomHeaderLabel(QWidget *parent)
    : QLabel(parent)
{
    setWordWrap(true);
    setTextInteractionFlags(Qt::TextBrowserInteraction);
    setTextFormat(Qt::RichText);
    setVisible(false);
    connect(this, &QLabel::linkActivated, this, &RoomHeaderLabel::slotMoreInfo);
    connect(this, &QLabel::linkHovered, this, [this](const QString &url) {
        if (url != "showlesstext"_L1 && url != "showmoretext"_L1) {
            setToolTip(url);
        }
    });
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
    QLabel::setText("<qt>"_L1 + text + "</qt>"_L1);
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
    } else {
        RuqolaUtils::self()->openUrl(QUrl(content));
    }
}

QString RoomHeaderLabel::rPixelSqueeze(const QString &text, int maxPixels) const
{
    const auto tSize = textSize(text);
    const int tHeight = tSize.height();
    if (tHeight > (3 * fontMetrics().ascent() + fontMetrics().descent())) {
        QString tmp = text;
        if (!mExpandTopic) {
            int tw = tSize.width();
            const QString showMoreText = i18n("(Show More Info…)");
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
                tmp = tmp.split(u"\n"_s).at(0);
            }
            if (!tmp.endsWith(u'\n')) {
                tmp.append(u'\n');
            }
            return tmp.append(u"<a href=\"showmoretext\"> %1</a>"_s.arg(showMoreText));
        } else {
            if (!tmp.endsWith(u'\n')) {
                tmp.append(u'\n');
            }
            return tmp.append(u"<a href=\"showlesstext\"> %1</a>"_s.arg(i18n("(Show Less Info…)")));
        }
    }
    return text;
}

QSize RoomHeaderLabel::textSize(const QString &text) const
{
    QTextDocument document;
    document.setDefaultFont(font());
    document.setHtml("<qt>"_L1 + text + "</qt>"_L1);

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
            mFullText += u'\n';
        }
        mFullText += mAnnouncement;
    }
    updateSqueezedText();
}

#include "moc_roomheaderlabel.cpp"
