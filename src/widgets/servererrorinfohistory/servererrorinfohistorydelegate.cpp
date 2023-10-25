/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfohistorydelegate.h"
#include "common/delegatepaintutil.h"
#include "config-ruqola.h"
#include "delegateutils/messagedelegateutils.h"
#include "delegateutils/textselectionimpl.h"
#include "model/servererrorinfohistorymodel.h"
#include "rocketchataccount.h"
#include "ruqola_sizehint_cache_debug.h"
#include "textconverter.h"
#include <QAbstractItemView>
#include <QPainter>
#include <QToolTip>

ServerErrorInfoHistoryDelegate::ServerErrorInfoHistoryDelegate(QListView *view, QObject *parent)
    : MessageListDelegateBase{view, parent}
{
}

ServerErrorInfoHistoryDelegate::~ServerErrorInfoHistoryDelegate() = default;

void ServerErrorInfoHistoryDelegate::drawAccountInfo(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    const QPen origPen = painter->pen();
    const qreal margin = MessageDelegateUtils::basicMargin();
    const QString accountName = index.data(ServerErrorInfoHistoryModel::AccountName).toString();
    const QString accountInfoStr = accountName;
    const QSize infoSize = option.fontMetrics.size(Qt::TextSingleLine, accountInfoStr);
    const QRect infoAreaRect(option.rect.x(), option.rect.y(), option.rect.width(), infoSize.height()); // the whole row
    const QRect infoTextRect = QStyle::alignedRect(Qt::LayoutDirectionAuto, Qt::AlignCenter, infoSize, infoAreaRect);
    painter->drawText(infoTextRect, accountInfoStr);
    const int lineY = (infoAreaRect.top() + infoAreaRect.bottom()) / 2;
    QColor lightColor(painter->pen().color());
    lightColor.setAlpha(60);
    painter->setPen(lightColor);
    painter->drawLine(infoAreaRect.left(), lineY, infoTextRect.left() - margin, lineY);
    painter->drawLine(infoTextRect.right() + margin, lineY, infoAreaRect.right(), lineY);
    painter->setPen(origPen);
}

void ServerErrorInfoHistoryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    drawBackground(painter, option, index);

    const Layout layout = doLayout(option, index);

    if (!layout.sameAccountAsPreviousMessage) {
        drawAccountInfo(painter, index, option);
    }

    // Draw Text
    if (layout.textRect.isValid()) {
        auto *doc = documentForModelIndex(index, layout.textRect.width());
        if (doc) {
            MessageDelegateUtils::drawSelection(doc,
                                                layout.textRect,
                                                layout.textRect.top(),
                                                painter,
                                                index,
                                                option,
                                                mTextSelectionImpl->textSelection(),
                                                {},
                                                false);
        }
    }

    // Timestamp
    DelegatePaintUtil::drawLighterText(painter, layout.timeStampText, layout.timeStampPos);

    // debug (TODO remove it for release)
    // painter->drawRect(option.rect.adjusted(0, 0, -1, -1));

    painter->restore();
}

QSize ServerErrorInfoHistoryDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#if USE_SIZEHINT_CACHE_SUPPORT
    const QString identifier = cacheIdentifier(index);
    auto it = mSizeHintCache.find(identifier);
    if (it != mSizeHintCache.end()) {
        const QSize result = it->value;
        qCDebug(RUQOLA_SIZEHINT_CACHE_LOG) << "ServerErrorInfoHistoryDelegate: SizeHint found in cache: " << result;
        return result;
    }
#endif
    // Note: option.rect in this method is huge (as big as the viewport)
    const Layout layout = doLayout(option, index);
    int additionalHeight = 0;
    // A little bit of margin below the very last item, it just looks better
    if (index.row() == index.model()->rowCount() - 1) {
        additionalHeight += 4;
    }
    // contents is date + text
    const int contentsHeight = layout.textRect.y() + layout.textRect.height() - option.rect.y();
    const QSize size = {option.rect.width(), contentsHeight + additionalHeight};
#if USE_SIZEHINT_CACHE_SUPPORT
    if (!size.isEmpty()) {
        mSizeHintCache.insert(identifier, size);
    }
#endif
    return size;
}

ServerErrorInfoHistoryDelegate::Layout ServerErrorInfoHistoryDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ServerErrorInfoHistoryDelegate::Layout layout;
    const QString accountName = index.data(ServerErrorInfoHistoryModel::AccountName).toString();
    const auto sameAccountAsPreviousMessage = [&] {
        if (index.row() < 1) {
            return false;
        }

        const auto previousIndex = index.siblingAtRow(index.row() - 1);
        const auto previousAccountName = previousIndex.data(ServerErrorInfoHistoryModel::AccountName).toString();
        return previousAccountName == accountName;
    }();

    // Timestamp
    layout.timeStampText = index.data(ServerErrorInfoHistoryModel::DateTimeStr).toString();
    layout.sameAccountAsPreviousMessage = sameAccountAsPreviousMessage;

    const int margin = MessageDelegateUtils::basicMargin();

    const int textLeft = margin;
    const QSize timeSize = MessageDelegateUtils::timeStampSize(layout.timeStampText, option);
    const int widthAfterMessage = margin + timeSize.width() + margin / 2;
    const int maxWidth = qMax(30, option.rect.width() - textLeft - widthAfterMessage);

    layout.baseLine = 0;
    const QSize textSize = textSizeHint(index, maxWidth, option, &layout.baseLine);

    const int textVMargin = 3; // adjust this for "compactness"
    QRect usableRect = option.rect;
    // Add area for account/room info

    if (!layout.sameAccountAsPreviousMessage) {
        usableRect.setTop(usableRect.top() + option.fontMetrics.height());
    }

    layout.textRect = QRect(textLeft, usableRect.top() + textVMargin, maxWidth, textSize.height() + textVMargin);
    layout.baseLine += layout.textRect.top(); // make it absolute

    layout.timeStampPos = QPoint(option.rect.width() - timeSize.width() - margin / 2, layout.baseLine);

    return layout;
}

QString ServerErrorInfoHistoryDelegate::cacheIdentifier(const QModelIndex &index) const
{
    const QString identifier = index.data(ServerErrorInfoHistoryModel::Identifier).toString();
    Q_ASSERT(!identifier.isEmpty());
    return identifier;
}

QTextDocument *ServerErrorInfoHistoryDelegate::documentForModelIndex(const QModelIndex &index, int width) const
{
    Q_ASSERT(index.isValid());
    const QString identifier = cacheIdentifier(index);
    Q_ASSERT(!identifier.isEmpty());
    auto it = mDocumentCache.find(identifier);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (width != -1 && !qFuzzyCompare(ret->textWidth(), width)) {
            ret->setTextWidth(width);
        }
        return ret;
    }

    const QString messageStr = index.data(ServerErrorInfoHistoryModel::MessageStr).toString();

    if (messageStr.isEmpty()) {
        return nullptr;
    }
    // Use TextConverter in case it starts with a [](URL) reply marker
    QString needUpdateMessageId; // TODO use it ?
    const TextConverter::ConvertMessageTextSettings settings(messageStr, {}, {}, QStringList(), nullptr, nullptr, {}, {}, mSearchText);
    int recursiveIndex = 0;
    const QString contextString = TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex);
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    mDocumentCache.insert(identifier, std::move(doc));
    return ret;
}

bool ServerErrorInfoHistoryDelegate::helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (!helpEvent || !view || !index.isValid()) {
        return QItemDelegate::helpEvent(helpEvent, view, option, index);
    }

    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }

    const Layout layout = doLayout(option, index);
    const auto *doc = documentForModelIndex(index, layout.textRect.width());
    if (!doc) {
        return false;
    }

    const QPoint relativePos = adaptMousePosition(helpEvent->pos(), layout.textRect, option);
    QString formattedTooltip;
    if (MessageDelegateUtils::generateToolTip(doc, relativePos, formattedTooltip)) {
        QToolTip::showText(helpEvent->globalPos(), formattedTooltip, view);
        return true;
    }
    return true;
}

QPoint ServerErrorInfoHistoryDelegate::adaptMousePosition(const QPoint &pos, QRect textRect, const QStyleOptionViewItem &option)
{
    Q_UNUSED(option);
    const QPoint relativePos = pos - textRect.topLeft();
    return relativePos;
}

bool ServerErrorInfoHistoryDelegate::mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto mev = static_cast<QMouseEvent *>(event);
        const Layout layout = doLayout(option, index);
        if (handleMouseEvent(mev, layout.textRect, option, index)) {
            return true;
        }
    } else if (eventType == QEvent::MouseButtonPress || eventType == QEvent::MouseMove || eventType == QEvent::MouseButtonDblClick) {
        auto mev = static_cast<QMouseEvent *>(event);
        if (mev->buttons() & Qt::LeftButton) {
            const Layout layout = doLayout(option, index);
            if (handleMouseEvent(mev, layout.textRect, option, index)) {
                return true;
            }
        }
    }
    return false;
}

bool ServerErrorInfoHistoryDelegate::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const Layout layout = doLayout(option, index);
    if (MessageListDelegateBase::maybeStartDrag(event, layout.textRect, option, index)) {
        return true;
    }
    return false;
}

RocketChatAccount *ServerErrorInfoHistoryDelegate::rocketChatAccount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return nullptr;
}

QString ServerErrorInfoHistoryDelegate::selectedText() const
{
    return mTextSelectionImpl->textSelection()->selectedText(TextSelection::Format::Text);
}

bool ServerErrorInfoHistoryDelegate::hasSelection() const
{
    return mTextSelectionImpl->textSelection()->hasSelection();
}

#include "moc_servererrorinfohistorydelegate.cpp"
