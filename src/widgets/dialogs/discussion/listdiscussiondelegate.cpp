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

#include "colors.h"
#include "common/delegatepaintutil.h"
#include "delegateutils/messagedelegateutils.h"
#include "model/discussionsmodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "textconverter.h"

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

    const QString discussionsText = i18n("Open Discussion");
    painter->setPen(Colors::self().schemeView().foreground(KColorScheme::LinkText).color());
    painter->drawText(DelegatePaintUtil::margin() + option.rect.x(), layout.openDiscussionTextY + painter->fontMetrics().ascent(), discussionsText);

    painter->restore();
}

bool ListDiscussionDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        const QString discussionRoomId = index.data(DiscussionsModel::DiscussionRoomId).toString();
        Q_EMIT openDiscussion(discussionRoomId);
        return true;
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

QTextDocument *ListDiscussionDelegate::documentForIndex(const QModelIndex &index, int width) const
{
    Q_ASSERT(index.isValid());
    const QString discussionRoomId = index.data(DiscussionsModel::DiscussionRoomId).toString();
    Q_ASSERT(!discussionRoomId.isEmpty());

    auto it = mDocumentCache.find(discussionRoomId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (width != -1 && !qFuzzyCompare(ret->textWidth(), width)) {
            ret->setTextWidth(width);
        }
        return ret;
    }

    const QString messageStr = index.data(DiscussionsModel::Description).toString();

    if (messageStr.isEmpty()) {
        return nullptr;
    }
    // Use TextConverter in case it starts with a [](URL) reply marker
    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    QString needUpdateMessageId; // TODO use it ?
    const QString contextString = TextConverter::convertMessageText(messageStr,
                                                                    rcAccount ? rcAccount->userName() : QString(),
                                                                    {},
                                                                    rcAccount ? rcAccount->highlightWords() : QStringList(),
                                                                    rcAccount ? rcAccount->emojiManager() : nullptr,
                                                                    rcAccount ? rcAccount->messageCache() : nullptr,
                                                                    needUpdateMessageId,
                                                                    {},
                                                                    {});
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    mDocumentCache.insert(discussionRoomId, std::move(doc));
    return ret;
}

QSize ListDiscussionDelegate::textSizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option, qreal *pBaseLine) const
{
    Q_UNUSED(option)
    auto *doc = documentForIndex(index, maxWidth);
    return MessageDelegateUtils::textSizeHint(doc, pBaseLine);
}
