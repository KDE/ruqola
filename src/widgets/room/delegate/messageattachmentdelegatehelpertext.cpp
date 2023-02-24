/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelpertext.h"
#include "colors.h"
#include "common/delegatepaintutil.h"
#include "delegateutils/messagedelegateutils.h"
#include "rocketchataccount.h"
#include "textconverter.h"

#include <KColorScheme>
#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QToolTip>

MessageAttachmentDelegateHelperText::MessageAttachmentDelegateHelperText(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageAttachmentDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageAttachmentDelegateHelperText::~MessageAttachmentDelegateHelperText() = default;

void MessageAttachmentDelegateHelperText::draw(const MessageAttachment &msgAttach,
                                               QPainter *painter,
                                               QRect messageRect,
                                               const QModelIndex &index,
                                               const QStyleOptionViewItem &option) const
{
    // painter->drawRect(messageRect);

    const TextLayout layout = layoutText(msgAttach, option, messageRect.width(), messageRect.height());
    // painter->drawRect(layout.titleRect.translated(messageRect.topLeft()));
    int nextY = messageRect.y();
    if (!layout.title.isEmpty()) {
        const QFont oldFont = painter->font();
        if (!msgAttach.link().isEmpty()) {
            painter->setPen(Colors::self().schemeView().foreground(KColorScheme::LinkText).color());
        }
        painter->setFont(layout.textFont);
        painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);
        painter->setFont(oldFont);
        const QIcon hideShowIcon = QIcon::fromTheme(layout.isShown ? QStringLiteral("visibility") : QStringLiteral("hint"));
        hideShowIcon.paint(painter, layout.hideShowButtonRect.translated(messageRect.topLeft()));
        nextY += layout.titleRect.height() + DelegatePaintUtil::margin();
    }
    if (layout.isShown || layout.title.isEmpty()) {
        auto *doc = documentAttachmentForIndex(msgAttach, messageRect.width());
        if (!doc) {
            return;
        }
        MessageDelegateUtils::drawSelection(doc, messageRect, nextY, painter, index, option, mSelectionImpl->textSelection(), msgAttach);
    }
}

QSize MessageAttachmentDelegateHelperText::sizeHint(const MessageAttachment &msgAttach,
                                                    const QModelIndex &index,
                                                    int maxWidth,
                                                    const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const TextLayout layout = layoutText(msgAttach, option, maxWidth, -1);
    int height = layout.titleRect.height();
    if ((layout.isShown && !layout.title.isEmpty()) || layout.title.isEmpty()) {
        height += layout.textSize.height() + DelegatePaintUtil::margin();
    }
    return {static_cast<int>(qMax(layout.titleRect.width(), static_cast<qreal>(maxWidth))), height};
}

QPoint MessageAttachmentDelegateHelperText::adaptMousePosition(const QPoint &pos,
                                                               const MessageAttachment &msgAttach,
                                                               QRect attachmentsRect,
                                                               const QStyleOptionViewItem &option)
{
    const TextLayout layout = layoutText(msgAttach, option, attachmentsRect.width(), attachmentsRect.height());
    const QPoint relativePos = pos - attachmentsRect.topLeft() - QPoint(0, layout.titleRect.height() + DelegatePaintUtil::margin());
    return relativePos;
}

bool MessageAttachmentDelegateHelperText::handleMouseEvent(const MessageAttachment &msgAttach,
                                                           QMouseEvent *mouseEvent,
                                                           QRect attachmentsRect,
                                                           const QStyleOptionViewItem &option,
                                                           const QModelIndex &index)
{
    const QEvent::Type eventType = mouseEvent->type();
    switch (eventType) {
    case QEvent::MouseButtonRelease: {
        const QPoint pos = mouseEvent->pos();
        const TextLayout layout = layoutText(msgAttach, option, attachmentsRect.width(), attachmentsRect.height());
        if (layout.hideShowButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            MessageModel::AttachmentVisibility attachmentVisibility;
            attachmentVisibility.show = !layout.isShown;
            attachmentVisibility.attachmentId = msgAttach.attachmentId();
            auto model = const_cast<QAbstractItemModel *>(index.model());
            model->setData(index, QVariant::fromValue(attachmentVisibility), MessageModel::DisplayAttachment);
            return true;
        }
        if (layout.titleRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            const QString link{msgAttach.link()};
            if (!link.isEmpty()) {
                Q_EMIT mRocketChatAccount->openLinkRequested(link);
                return true;
            }
        }
        // Clicks on links
        auto *doc = documentAttachmentForIndex(msgAttach, attachmentsRect.width());
        if (doc) {
            // Fix mouse position (we have layout.titleSize.height() + DelegatePaintUtil::margin() too)
            const QPoint mouseClickPos = pos - attachmentsRect.topLeft() - QPoint(0, layout.titleRect.height() + DelegatePaintUtil::margin());
            const QString link = doc->documentLayout()->anchorAt(mouseClickPos);
            if (!link.isEmpty()) {
                Q_EMIT mRocketChatAccount->openLinkRequested(link);
                return true;
            }
        }
        // don't return true here, we need to send mouse release events to other helpers (ex: click on image)
        break;
    }
    default:
        break;
    }

    return MessageAttachmentDelegateHelperBase::handleMouseEvent(msgAttach, mouseEvent, attachmentsRect, option, index);
}

MessageAttachmentDelegateHelperText::TextLayout MessageAttachmentDelegateHelperText::layoutText(const MessageAttachment &msgAttach,
                                                                                                const QStyleOptionViewItem &option,
                                                                                                int attachmentsWidth,
                                                                                                int attachmentsHeight) const
{
    Q_UNUSED(attachmentsHeight)
    TextLayout layout;
    layout.title = msgAttach.title();
    if (!layout.title.isEmpty()) {
        layout.textFont = option.font;
        layout.textFont.setBold(true);
        const QFontMetricsF textFontMetrics(layout.textFont);

        const auto titleSize = textFontMetrics.size(Qt::TextSingleLine, layout.title);
        const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
        layout.hideShowButtonRect = QRect(titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
        layout.titleRect = QRectF(QPoint(/*DelegatePaintUtil::margin()*/ 0, 0), titleSize);
    }
    layout.isShown = msgAttach.showAttachment();
    auto *doc = documentAttachmentForIndex(msgAttach, attachmentsWidth);
    layout.textSize = doc ? QSize(doc->idealWidth(), doc->size().height()) : QSize();
    return layout;
}

QTextDocument *MessageAttachmentDelegateHelperText::documentAttachmentForIndex(const MessageAttachment &msgAttach, int width) const
{
    const QString attachmentId = msgAttach.attachmentId();
    auto it = mDocumentCache.find(attachmentId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (width != -1 && !qFuzzyCompare(ret->textWidth(), width)) {
            ret->setTextWidth(width);
        }
        return ret;
    }

    const QString text = msgAttach.text();
    if (text.isEmpty()) {
        const QString authorName = msgAttach.authorName();
        if (authorName.isEmpty()) {
            if (msgAttach.attachmentFieldsText().isEmpty()) {
                return nullptr;
            } else {
                const QString contextString = msgAttach.attachmentFieldsText();
                auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
                auto ret = doc.get();
                mDocumentCache.insert(attachmentId, std::move(doc));
                return ret;
            }
        } else {
            // Add support for icon_author too
            const QString contextString = QStringLiteral("<a href=\"%1\">%2</a>").arg(msgAttach.link(), authorName) + msgAttach.attachmentFieldsText();
            auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
            auto ret = doc.get();
            mDocumentCache.insert(attachmentId, std::move(doc));
            return ret;
        }
    } else {
        // Use TextConverter in case it starts with a [](URL) reply marker
        auto *rcAccount = mRocketChatAccount;
        QString needUpdateMessageId;
        int maximumRecursiveQuotedText = -1;
        if (rcAccount) {
            maximumRecursiveQuotedText = rcAccount->ruqolaServerConfig()->messageQuoteChainLimit();
        }
        const TextConverter::ConvertMessageTextSettings settings(text,
                                                                 rcAccount->userName(),
                                                                 {},
                                                                 rcAccount->highlightWords(),
                                                                 rcAccount->emojiManager(),
                                                                 rcAccount->messageCache(),
                                                                 {},
                                                                 {},
                                                                 {},
                                                                 maximumRecursiveQuotedText);

        // TODO use needUpdateIndex ?
        int recursiveIndex = 0;
        const QString contextString = TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex) + msgAttach.attachmentFieldsText();
        auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
        auto ret = doc.get();
        mDocumentCache.insert(attachmentId, std::move(doc));
        return ret;
    }
}

bool MessageAttachmentDelegateHelperText::handleHelpEvent(QHelpEvent *helpEvent,
                                                          QRect messageRect,
                                                          const MessageAttachment &msgAttach,
                                                          const QStyleOptionViewItem &option)
{
    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }

    const TextLayout layout = layoutText(msgAttach, option, messageRect.width(), messageRect.height());
    if (layout.titleRect.translated(messageRect.topLeft()).contains(helpEvent->pos())) {
        const QString msgAttachLink{msgAttach.link()};
        if (!msgAttachLink.isEmpty()) {
            QString formattedTooltip;
            MessageDelegateUtils::generateToolTip(QString(), msgAttachLink, formattedTooltip);
            QToolTip::showText(helpEvent->globalPos(), formattedTooltip, mListView);
            return true;
        }
    }

    const auto *doc = documentAttachmentForIndex(msgAttach, messageRect.width());
    if (!doc) {
        return false;
    }

    const QPoint pos = helpEvent->pos() - messageRect.topLeft() - QPoint(0, layout.titleRect.height() + DelegatePaintUtil::margin());
    QString formattedTooltip;
    if (MessageDelegateUtils::generateToolTip(doc, pos, formattedTooltip)) {
        QToolTip::showText(helpEvent->globalPos(), formattedTooltip);
        return true;
    }
    return true;
}
