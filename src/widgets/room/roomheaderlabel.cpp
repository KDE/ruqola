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

#include <QTextDocument>

RoomHeaderLabel::RoomHeaderLabel(QWidget *parent)
    : QLabel(parent)
{
    setWordWrap(true);
    setTextInteractionFlags(Qt::TextBrowserInteraction);
}

RoomHeaderLabel::~RoomHeaderLabel()
{
}

QSize RoomHeaderLabel::minimumSizeHint() const
{
    return QLabel::minimumSizeHint();
}

QSize RoomHeaderLabel::sizeHint() const
{
    return QLabel::sizeHint();
}

void RoomHeaderLabel::resizeEvent(QResizeEvent *ev)
{
    QLabel::resizeEvent(ev);
    updateSqueezedText();
}

void RoomHeaderLabel::setLabelText(const QString &text)
{
    if (text != mFullText) {
        mFullText = text;
        updateSqueezedText();
    }
}

void RoomHeaderLabel::updateSqueezedText()
{
    setToolTip(QString());

    if (mFullText.isEmpty()) {
        QLabel::setText(QString());

        return;
    }

    QString text = mFullText;

    if (height() < ((fontMetrics().ascent() + fontMetrics().descent()) * 2)) {
        text = rPixelSqueeze(text, width() - 10);
        setWordWrap(false);
    } else {
        setWordWrap(true);
    }
    QLabel::setText(QLatin1String("<qt>") + text + QLatin1String("</qt>"));
}

QString RoomHeaderLabel::rPixelSqueeze(const QString &text, int maxPixels) const
{
    int tw = textWidth(text);

    if (tw > maxPixels) {
        QString tmp = text;
        int em = fontMetrics().maxWidth();
        maxPixels -= fontMetrics().horizontalAdvance(QStringLiteral("..."));
        int len, delta;

        // On some MacOS system, maxWidth may return 0
        if (em == 0) {
            for (QChar c : text) {
                em = qMax(em, fontMetrics().horizontalAdvance(c));
            }
        }

        while ((tw > maxPixels) && !tmp.isEmpty()) {
            len = tmp.length();
            delta = (tw - maxPixels) / em;
            delta = qBound(1, delta, len);

            tmp.remove(len - delta, delta);
            tw = textWidth(tmp);
        }

        return tmp.append(QLatin1String("..."));
    }

    return text;
}

int RoomHeaderLabel::textWidth(const QString &text) const
{
    QTextDocument document;
    document.setDefaultFont(font());
    document.setHtml(QLatin1String("<qt>") + text + QLatin1String("</qt>"));

    return document.size().toSize().width();
}
