/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#include "messagedelegatehelperreactions.h"
#include <model/messagemodel.h>
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolautils.h"
#include "emoticons/emojimanager.h"
#include "emoticons/unicodeemoticon.h"

#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QStyleOptionViewItem>
#include <ruqola.h>
#include <QToolTip>

constexpr const qreal margin = 8;

MessageDelegateHelperReactions::MessageDelegateHelperReactions()
    : mEmojiFont(QStringLiteral("NotoColorEmoji"))
{
}

QVector<MessageDelegateHelperReactions::ReactionLayout> MessageDelegateHelperReactions::layoutReactions(const QVector<Reaction> &reactions, const QRect &reactionsRect, const QStyleOptionViewItem &option) const
{
    QVector<ReactionLayout> layouts;
    layouts.reserve(reactions.count());
    auto *emojiManager = Ruqola::self()->rocketChatAccount()->emojiManager();
    QFontMetricsF emojiFontMetrics(mEmojiFont);
    const qreal smallMargin = margin/2.0;
    qreal x = reactionsRect.x();

    for (const Reaction &reaction : reactions) {
        ReactionLayout layout;
        layout.emojiString = emojiManager->unicodeEmoticonForEmoji(reaction.reactionName()).unicode();
        qreal emojiWidth;
        if (layout.emojiString.isEmpty()) {
            layout.emojiString = reaction.reactionName(); // ugly fallback: ":1md"
            emojiWidth = option.fontMetrics.horizontalAdvance(layout.emojiString) + smallMargin;
            layout.useEmojiFont = false;
        } else {
            emojiWidth = emojiFontMetrics.horizontalAdvance(layout.emojiString);
            layout.useEmojiFont = true;
        }
        layout.countStr = QString::number(reaction.count());
        const int countWidth = option.fontMetrics.horizontalAdvance(layout.countStr) + smallMargin;
        layout.reactionRect = QRectF(x, reactionsRect.y(),
                                     emojiWidth + countWidth + margin, reactionsRect.height());
        layout.emojiOffset = smallMargin + 1;
        layout.countRect = layout.reactionRect.adjusted(layout.emojiOffset + emojiWidth, smallMargin, 0, 0);
        layout.reaction = reaction;

        layouts.append(layout);
        x += layout.reactionRect.width() + margin;
    }
    return layouts;
}

void MessageDelegateHelperReactions::draw(QPainter *painter, const QRect &reactionsRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const QVector<Reaction> reactions = message->reactions().reactions();
    if (reactions.isEmpty()) {
        return;
    }

    const QVector<ReactionLayout> layouts = layoutReactions(reactions, reactionsRect, option);

    const QPen origPen = painter->pen();
    const QBrush origBrush = painter->brush();
    const QPen buttonPen(option.palette.color(QPalette::Highlight).darker());
    QColor backgroundColor = option.palette.color(QPalette::Highlight);
    backgroundColor.setAlpha(60);
    const QBrush buttonBrush(backgroundColor);
    const qreal smallMargin = 4;
    for (const ReactionLayout &reactionLayout : layouts) {
        if (!reactionLayout.emojiString.isEmpty()) {
            const QRectF reactionRect = reactionLayout.reactionRect;

            // Rounded rect
            painter->setPen(buttonPen);
            painter->setBrush(buttonBrush);
            painter->drawRoundedRect(reactionRect, 5, 5);
            painter->setBrush(origBrush);
            painter->setPen(origPen);

            // Emoji
            if (reactionLayout.useEmojiFont) {
                painter->setFont(mEmojiFont);
            }
            painter->drawText(reactionRect.adjusted(reactionLayout.emojiOffset, smallMargin, 0, 0), reactionLayout.emojiString);

            // Count
            painter->setFont(option.font);
            painter->drawText(reactionLayout.countRect, reactionLayout.countStr);
        }
    }
}

QSize MessageDelegateHelperReactions::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    int reactionsHeight = 0;
    const QVector<Reaction> reactions = message->reactions().reactions();
    if (!reactions.isEmpty()) {
        QFontMetrics emojiFontMetrics(mEmojiFont);
        reactionsHeight = qMax<qreal>(emojiFontMetrics.height(), option.fontMetrics.height()) + margin;
    }
    return QSize(maxWidth, reactionsHeight);
}

bool MessageDelegateHelperReactions::handleMouseEvent(QMouseEvent *mouseEvent, const QRect &reactionsRect, const QStyleOptionViewItem &option, const Message *message)
{
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const QPoint pos = mouseEvent->pos();
        const QVector<ReactionLayout> reactions = layoutReactions(message->reactions().reactions(), reactionsRect, option);
        for (const ReactionLayout &reactionLayout : reactions) {
            if (reactionLayout.reactionRect.contains(pos)) {
                const Reaction &reaction = reactionLayout.reaction;
                auto *rcAccount = Ruqola::self()->rocketChatAccount();
                const bool doAdd = !reaction.userNames().contains(rcAccount->userName());
                rcAccount->reactOnMessage(message->messageId(), reaction.reactionName(), doAdd);
                return true;
            }
        }
    }
    return false;
}

bool MessageDelegateHelperReactions::handleHelpEvent(QHelpEvent *helpEvent, QWidget *view, const QRect &reactionsRect, const QStyleOptionViewItem &option, const Message *message)
{
    const QVector<ReactionLayout> reactions = layoutReactions(message->reactions().reactions(), reactionsRect, option);
    for (const ReactionLayout &reactionLayout : reactions) {
        if (reactionLayout.reactionRect.contains(helpEvent->pos())) {
            const Reaction &reaction = reactionLayout.reaction;
            const QString tooltip = reaction.convertedUsersNameAtToolTip();
            QToolTip::showText(helpEvent->globalPos(), tooltip, view);
            return true;
        }
    }
    return false;
}
