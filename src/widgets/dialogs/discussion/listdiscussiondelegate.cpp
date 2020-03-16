/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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
#include "listdiscussiondelegate.h"
#include <QPainter>
#include <QMouseEvent>
#include "model/discussionsmodel.h"

ListDiscussionDelegate::ListDiscussionDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

ListDiscussionDelegate::~ListDiscussionDelegate()
{
}

static void drawTimestamp(QPainter *painter, const QString &timeStampText, const QPoint &timeStampPos)
{
    const QPen oldPen = painter->pen();
    QColor col = painter->pen().color();
    col.setAlpha(128); // TimestampText.qml had opacity: .5
    painter->setPen(col);
    painter->drawText(timeStampPos, timeStampText);
    painter->setPen(oldPen);
}

static qreal basicMargin()
{
    return 8;
}

// [date]
// text
// number of discussion + last date
// add text for opening dicussion.

void ListDiscussionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    const int margin = 8;

    const QRect decorationRect(option.rect.x() + margin, option.rect.y(), iconSize, option.rect.height());

    const QString text = index.data(DiscussionsModel::Description).toString();

    //TODO improve it.
    const int xText = option.rect.x() + iconSize + 2 * margin;
    const QRect displayRect(xText, option.rect.y(),
                            option.rect.width() - xText - margin,
                            option.rect.height());

    QStyleOptionViewItem optionCopy = option;
    optionCopy.showDecorationSelected = true;
    drawBackground(painter, optionCopy, index);
    const QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    icon.paint(painter, decorationRect, Qt::AlignCenter);

    drawDisplay(painter, optionCopy, displayRect, text); // this takes care of eliding if the text is too long
}

bool ListDiscussionDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto *mev = static_cast<QMouseEvent *>(event);
        //TODO
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}

QSize ListDiscussionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Note: option.rect in this method is huge (as big as the viewport)
    const Layout layout = doLayout(option, index);

    int additionalHeight = 0;
    // A little bit of margin below the very last item, it just looks better
    if (index.row() == index.model()->rowCount() - 1) {
        additionalHeight += 4;
    }

//    const int contentsHeight = layout.timeStampY /* + layout.senderY + layout.attachmentNameY*/ - option.rect.y();

    return QSize(option.rect.width(),
                 /*contentsHeight +*/ additionalHeight);
}

ListDiscussionDelegate::Layout ListDiscussionDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Layout layout;
    QRect usableRect = option.rect;
    layout.usableRect = usableRect; // Just for the top, for now. The left will move later on.
    return layout;
}
