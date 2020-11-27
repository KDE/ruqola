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
#include <QDebug>
#include <KColorScheme>
#include <KLocalizedString>

#include "model/discussionsmodel.h"
#include "common/delegatepaintutil.h"

ListDiscussionDelegate::ListDiscussionDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

ListDiscussionDelegate::~ListDiscussionDelegate()
{
}

// [date]
// text
// number of discussion + last date
// add text for opening dicussion.

void ListDiscussionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    const Layout layout = doLayout(option, index);

    // Draw the sender (below the filename)
    painter->drawText(DelegatePaintUtil::margin() + option.rect.x(),
                      layout.textY + painter->fontMetrics().ascent(),
                      layout.text);

    // Draw the timestamp (below the sender)
    DelegatePaintUtil::drawTimestamp(painter, layout.lastMessageTimeText,
                                     QPoint(DelegatePaintUtil::margin() + option.rect.x(), layout.lastMessageTimeY + painter->fontMetrics().ascent()));

    KColorScheme scheme;
    const QString discussionsText = i18n("Open Discussion");
    painter->setPen(scheme.foreground(KColorScheme::LinkText).color());
    painter->drawText(DelegatePaintUtil::margin() + option.rect.x(), layout.openDiscussionTextY + painter->fontMetrics().ascent(), discussionsText);
    // Note: pen still blue, currently relying on restore()

    //TODO add open discussion text.

    painter->restore();
}

bool ListDiscussionDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto mev = static_cast<QMouseEvent *>(event);
        //TODO check if we muse open discussion
        qDebug() << " open discussion not implemented yet";
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}

QSize ListDiscussionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Note: option.rect in this method is huge (as big as the viewport)
    const Layout layout = doLayout(option, index);

    const int contentsHeight = layout.openDiscussionTextY  + option.fontMetrics.height() - option.rect.y();
    return QSize(option.rect.width(),
                 contentsHeight);
}

ListDiscussionDelegate::Layout ListDiscussionDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Layout layout;
    QRect usableRect = option.rect;
    layout.usableRect = usableRect; // Just for the top, for now. The left will move later on.

    layout.text = index.data(DiscussionsModel::Description).toString();
    layout.textY = usableRect.top();

    layout.lastMessageTimeText = index.data(DiscussionsModel::LastMessage).toString();
    layout.lastMessageTimeY = layout.textY + option.fontMetrics.height();

    layout.openDiscussionTextY = layout.lastMessageTimeY + option.fontMetrics.height();
    return layout;
}
