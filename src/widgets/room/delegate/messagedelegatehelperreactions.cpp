/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperreactions.h"
#include "common/delegatepaintutil.h"
#include "emoticons/emojimanager.h"
#include "model/messagemodel.h"
#include "rocketchataccount.h"
#include "ruqolaglobalconfig.h"
#include "utils.h"

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QMovie>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QToolTip>
#include <ruqola.h>

MessageDelegateHelperReactions::MessageDelegateHelperReactions(RocketChatAccount *account)
    : mEmojiFont(Utils::emojiFontName())
    , mRocketChatAccount(account)
{
}

QVector<MessageDelegateHelperReactions::ReactionLayout>
MessageDelegateHelperReactions::layoutReactions(const QVector<Reaction> &reactions, QRect reactionsRect, const QStyleOptionViewItem &option) const
{
    QVector<ReactionLayout> layouts;
    layouts.reserve(reactions.count());
    auto *emojiManager = mRocketChatAccount->emojiManager();
    const QFontMetricsF emojiFontMetrics(mEmojiFont);
    const qreal smallMargin = DelegatePaintUtil::margin() / 2.0;
    qreal x = reactionsRect.x();
    qreal y = reactionsRect.y();

    for (const Reaction &reaction : reactions) {
        ReactionLayout layout;
        layout.emojiString = emojiManager->unicodeEmoticonForEmoji(reaction.reactionName()).unicode();
        qreal emojiWidth = 0;
        if (!layout.emojiString.isEmpty()) {
            emojiWidth = emojiFontMetrics.horizontalAdvance(layout.emojiString);
            layout.useEmojiFont = true;
        } else {
            const QString fileName = emojiManager->customEmojiFileName(reaction.reactionName());
            if (!fileName.isEmpty()) {
                const QUrl emojiUrl = mRocketChatAccount->attachmentUrlFromLocalCache(fileName);
                if (emojiUrl.isEmpty()) {
                    // The download is happening, this will all be updated again later
                } else {
                    if (!mPixmapCache.pixmapForLocalFile(emojiUrl.toLocalFile()).isNull()) {
                        layout.emojiImagePath = emojiUrl.toLocalFile();
                        const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
                        emojiWidth = iconSize;
                    }
                }
            }
            if (layout.emojiImagePath.isEmpty()) {
                layout.emojiString = reaction.reactionName(); // ugly fallback: ":1md"
                emojiWidth = option.fontMetrics.horizontalAdvance(layout.emojiString) + smallMargin;
            }
            layout.useEmojiFont = false;
        }
        layout.countStr = QString::number(reaction.count());
        const int countWidth = option.fontMetrics.horizontalAdvance(layout.countStr) + smallMargin;
        // [reactionRect] = [emojiOffset (margin)] [emojiWidth] [countWidth] [margin/2]
        layout.reactionRect = QRectF(x, y, emojiWidth + countWidth + DelegatePaintUtil::margin(), reactionsRect.height());
        layout.emojiOffset = smallMargin + 1;
        layout.countRect = layout.reactionRect.adjusted(layout.emojiOffset + emojiWidth, smallMargin, 0, 0);
        layout.reaction = reaction;

        layouts.append(layout);
        x += layout.reactionRect.width() + DelegatePaintUtil::margin();
        if (x > reactionsRect.width()) {
            x = reactionsRect.x();
            y += reactionsRect.height() + DelegatePaintUtil::margin();
        }
    }
    return layouts;
}

void MessageDelegateHelperReactions::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}

void MessageDelegateHelperReactions::draw(QPainter *painter, QRect reactionsRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const QVector<Reaction> reactions = message->reactions().reactions();
    if (reactions.isEmpty()) {
        return;
    }
#if 0
    painter->save();
    painter->setPen(Qt::red);
    painter->drawRect(reactionsRect);
    painter->restore();
#endif
    const QVector<ReactionLayout> layouts = layoutReactions(reactions, reactionsRect, option);

    const QPen origPen = painter->pen();
    const QBrush origBrush = painter->brush();
    const QPen buttonPen(option.palette.color(QPalette::Highlight).darker());
    QColor backgroundColor = option.palette.color(QPalette::Highlight);
    backgroundColor.setAlpha(60);
    const QBrush buttonBrush(backgroundColor);
    const qreal smallMargin = 4;
    for (const ReactionLayout &reactionLayout : layouts) {
        Q_ASSERT(!reactionLayout.emojiString.isEmpty() || !reactionLayout.emojiImagePath.isEmpty());
        const QRectF reactionRect = reactionLayout.reactionRect;

        // Rounded rect
        painter->setPen(buttonPen);
        painter->setBrush(buttonBrush);
        painter->drawRoundedRect(reactionRect.adjusted(0, 0, -1, -1), 5, 5);
        painter->setBrush(origBrush);
        painter->setPen(origPen);

        // Emoji
        const QRectF r = reactionRect.adjusted(reactionLayout.emojiOffset, smallMargin, 0, 0);
        if (!reactionLayout.emojiString.isEmpty()) {
            if (reactionLayout.useEmojiFont) {
                painter->setFont(mEmojiFont);
            }
            painter->drawText(r, reactionLayout.emojiString);
        } else {
            if (reactionLayout.reaction.isAnimatedImage() && RuqolaGlobalConfig::self()->animateGifImage()) {
                const int maxIconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);

                QPixmap scaledPixmap;
                auto it = findRunningAnimatedImage(index);
                if (it != mRunningAnimatedImages.end()) {
                    scaledPixmap = (*it).movie->currentPixmap();
                } else {
                    mRunningAnimatedImages.emplace_back(index);
                    auto &rai = mRunningAnimatedImages.back();
                    rai.movie->setFileName(reactionLayout.emojiImagePath);
                    rai.movie->setScaledSize(QSize(maxIconSize, maxIconSize));
                    auto view = qobject_cast<QAbstractItemView *>(const_cast<QWidget *>(option.widget));
                    const QPersistentModelIndex &idx = rai.index;
                    QObject::connect(
                        rai.movie,
                        &QMovie::frameChanged,
                        view,
                        [view, idx, this]() {
                            if (view->viewport()->rect().contains(view->visualRect(idx))) {
                                view->update(idx);
                            } else {
                                removeRunningAnimatedImage(idx);
                            }
                        },
                        Qt::QueuedConnection);
                    rai.movie->start();
                    scaledPixmap = rai.movie->currentPixmap();
                }
                scaledPixmap.setDevicePixelRatio(option.widget->devicePixelRatioF());
                painter->drawPixmap(r.x(), r.y(), scaledPixmap);
            } else {
                const QPixmap pixmap = mPixmapCache.pixmapForLocalFile(reactionLayout.emojiImagePath);
                const int maxIconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
                const QPixmap scaledPixmap = pixmap.scaled(maxIconSize, maxIconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                painter->drawPixmap(r.x(), r.y(), scaledPixmap);
            }
        }
        // Count
        painter->setFont(option.font);
        painter->drawText(reactionLayout.countRect, reactionLayout.countStr);
    }
}

std::vector<RunningAnimatedImage>::iterator MessageDelegateHelperReactions::findRunningAnimatedImage(const QModelIndex &index) const
{
    auto matchesIndex = [&](const RunningAnimatedImage &rai) {
        return rai.index == index;
    };
    return std::find_if(mRunningAnimatedImages.begin(), mRunningAnimatedImages.end(), matchesIndex);
}

void MessageDelegateHelperReactions::removeRunningAnimatedImage(const QModelIndex &index) const
{
    auto it = findRunningAnimatedImage(index);
    if (it != mRunningAnimatedImages.end()) {
        mRunningAnimatedImages.erase(it);
    }
}

QSize MessageDelegateHelperReactions::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    int reactionsHeight = 0;
    const QVector<Reaction> reactions = message->reactions().reactions();
    if (!reactions.isEmpty()) {
        const QFontMetrics emojiFontMetrics(mEmojiFont);
        reactionsHeight = qMax<qreal>(emojiFontMetrics.height(), option.fontMetrics.height()) + DelegatePaintUtil::margin();
    }
    return {maxWidth, reactionsHeight};
}

bool MessageDelegateHelperReactions::handleMouseEvent(QMouseEvent *mouseEvent, QRect reactionsRect, const QStyleOptionViewItem &option, const Message *message)
{
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const QPoint pos = mouseEvent->pos();
        const QVector<ReactionLayout> reactions = layoutReactions(message->reactions().reactions(), reactionsRect, option);
        for (const ReactionLayout &reactionLayout : reactions) {
            if (reactionLayout.reactionRect.contains(pos)) {
                const Reaction &reaction = reactionLayout.reaction;
                const bool doAdd = !reaction.userNames().contains(mRocketChatAccount->userName());
                mRocketChatAccount->reactOnMessage(message->messageId(), reaction.reactionName(), doAdd);
                return true;
            }
        }
    }
    return false;
}

bool MessageDelegateHelperReactions::handleHelpEvent(QHelpEvent *helpEvent,
                                                     QWidget *view,
                                                     QRect reactionsRect,
                                                     const QStyleOptionViewItem &option,
                                                     const Message *message)
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
