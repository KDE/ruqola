/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "listdiscussiondelegate.h"
#include <KColorScheme>
#include <KLocalizedString>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

#include "common/delegatepaintutil.h"
#include "model/discussionsmodel.h"

ListDiscussionDelegate::ListDiscussionDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

ListDiscussionDelegate::~ListDiscussionDelegate() = default;

// [date]
// text
// number of discussion + last date
// add text for opening dicussion.

void ListDiscussionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    const Layout layout = doLayout(option, index);

    // Draw the sender (below the filename)
    painter->drawText(DelegatePaintUtil::margin() + option.rect.x(), layout.textY + painter->fontMetrics().ascent(), layout.text);

    // Draw the number of message + timestamp (below the sender)
    const QString messageStr = i18np("%1 message", "%1 messages", layout.numberOfMessages) + QLatin1Char(' ') + layout.lastMessageTimeText;
    DelegatePaintUtil::drawLighterText(painter,
                                       messageStr,
                                       QPoint(DelegatePaintUtil::margin() + option.rect.x(), layout.lastMessageTimeY + painter->fontMetrics().ascent()));

    KColorScheme scheme;
    const QString discussionsText = i18n("Open Discussion");
    painter->setPen(scheme.foreground(KColorScheme::LinkText).color());
    painter->drawText(DelegatePaintUtil::margin() + option.rect.x(), layout.openDiscussionTextY + painter->fontMetrics().ascent(), discussionsText);
    // Note: pen still blue, currently relying on restore()

    // TODO add open discussion text.

    painter->restore();
}

bool ListDiscussionDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto mev = static_cast<QMouseEvent *>(event);
        // TODO check if we must open discussion
        qDebug() << " open discussion not implemented yet";
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}

QSize ListDiscussionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Note: option.rect in this method is huge (as big as the viewport)
    const Layout layout = doLayout(option, index);

    const int contentsHeight = layout.openDiscussionTextY + option.fontMetrics.height() - option.rect.y();
    return {option.rect.width(), contentsHeight};
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

    layout.numberOfMessages = index.data(DiscussionsModel::NumberOfMessages).toInt();

    layout.openDiscussionTextY = layout.lastMessageTimeY + option.fontMetrics.height();
    return layout;
}
