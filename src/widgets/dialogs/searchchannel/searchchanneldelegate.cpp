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
#include "searchchanneldelegate.h"
#include "common/delegatepaintutil.h"
#include <KLocalizedString>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QApplication>
#include <QStyle>
#include "model/searchchannelmodel.h"

SearchChannelDelegate::SearchChannelDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mJoinLabel(i18n("Join"))
{
}

SearchChannelDelegate::~SearchChannelDelegate()
{
}

void SearchChannelDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] <icon> [M] <name> [M] <add channel icon> [M]   ([M] = margin)
    //TODO add channel type icon too
    painter->save();
    const Layout layout = doLayout(option, index);
    QStyleOptionViewItem optionCopy = option;
    optionCopy.showDecorationSelected = true;

    drawBackground(painter, optionCopy, index);

    const QIcon icon = index.data(SearchChannelModel::IconName).value<QIcon>();
    icon.paint(painter, layout.joinButtonRect, Qt::AlignCenter);

    painter->drawText(layout.usableRect, layout.channelName);

    QStyleOptionButton buttonOpt;
    buttonOpt.rect = layout.selectChannelRect;
    buttonOpt.state = option.state;
    buttonOpt.text = mJoinLabel;
    buttonOpt.palette = option.palette;

    QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOpt, painter);

    painter->restore();
}

bool SearchChannelDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto mev = static_cast<QMouseEvent *>(event);
        const Layout layout = doLayout(option, index);
        if (layout.selectChannelRect.contains(mev->pos())) {
            Q_EMIT channelSelected(index);
            return true;
        }
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}

SearchChannelDelegate::Layout SearchChannelDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Layout layout;
    QRect usableRect = option.rect;
    layout.usableRect = usableRect; // Just for the top, for now. The left will move later on.

    const int joinLabelWidth = option.fontMetrics.boundingRect(mJoinLabel).width() + 2 * DelegatePaintUtil::margin();
    layout.joinButtonRect = QRect(option.rect.x() + DelegatePaintUtil::margin(), option.rect.y(), joinLabelWidth, option.rect.height());

    layout.channelName = index.data(SearchChannelModel::ChannelName).toString();

    layout.selectChannelRect = QRect(option.rect.width() - joinLabelWidth - DelegatePaintUtil::margin(), option.rect.y(), joinLabelWidth, option.rect.height());

    layout.usableRect.setLeft(layout.joinButtonRect.width() + DelegatePaintUtil::margin());

    return layout;
}

QSize SearchChannelDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    const int contentsHeight = option.fontMetrics.height() + DelegatePaintUtil::margin() - option.rect.y();
    return QSize(option.rect.width(),
                 contentsHeight);
}
