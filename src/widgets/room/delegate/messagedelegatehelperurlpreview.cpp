/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperurlpreview.h"

#include "common/delegatepaintutil.h"
#include "delegateutils/messagedelegateutils.h"
#include "messages/messageurl.h"
#include "rocketchataccount.h"
#include "textconverter.h"

#include <KLocalizedString>

#include <QListView>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QToolTip>

MessageDelegateHelperUrlPreview::MessageDelegateHelperUrlPreview(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageDelegateHelperUrlPreview::~MessageDelegateHelperUrlPreview() = default;

void MessageDelegateHelperUrlPreview::draw(const MessageUrl &messageUrl,
                                           QPainter *painter,
                                           QRect previewRect,
                                           const QModelIndex &index,
                                           const QStyleOptionViewItem &option) const
{
    const PreviewLayout layout = layoutPreview(messageUrl, option, previewRect.width(), previewRect.height());
    const QFont oldFont = painter->font();
    const QPen origPen = painter->pen();
    QColor lightColor(painter->pen().color());
    lightColor.setAlpha(60);
    painter->setPen(lightColor);
    QFont italicFont = oldFont;
    italicFont.setItalic(true);
    italicFont.setBold(true);
    painter->setFont(italicFont);
    painter->drawText(previewRect.x(), previewRect.y() + option.fontMetrics.ascent(), layout.previewTitle);
    painter->setFont(oldFont);
    painter->setPen(origPen);

    const QIcon hideShowIcon = QIcon::fromTheme(layout.isShown ? QStringLiteral("visibility") : QStringLiteral("hint"));
    hideShowIcon.paint(painter, layout.hideShowButtonRect.translated(previewRect.topLeft()));
    if (layout.isShown) {
        int nextY = previewRect.y() + option.fontMetrics.ascent() + DelegatePaintUtil::margin();
        if (!layout.pixmap.isNull()) {
            QPixmap scaledPixmap;
            scaledPixmap = layout.pixmap.scaled(layout.imageSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            painter->drawPixmap(previewRect.x(), nextY, scaledPixmap);
            nextY += scaledPixmap.height() / scaledPixmap.devicePixelRatioF() + DelegatePaintUtil::margin();
        }
        // qDebug() << " nextY " << nextY;
        drawDescription(messageUrl, previewRect, painter, nextY, index, option);
    }
}

MessageDelegateHelperUrlPreview::PreviewLayout MessageDelegateHelperUrlPreview::layoutPreview(const MessageUrl &messageUrl,
                                                                                              const QStyleOptionViewItem &option,
                                                                                              int urlsPreviewWidth,
                                                                                              int urlsPreviewHeight) const
{
    MessageDelegateHelperUrlPreview::PreviewLayout layout;
    layout.previewTitle = i18n("Link Preview");
    layout.previewTitleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.previewTitle);
    layout.hasDescription = messageUrl.hasHtmlDescription();
    const QUrl previewImageUrl = mRocketChatAccount ? mRocketChatAccount->previewUrlFromLocalCache(messageUrl.imageUrl()) : QUrl{};
    if (previewImageUrl.isLocalFile()) {
        layout.imageUrl = messageUrl.imageUrl();
    }
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    layout.hideShowButtonRect = QRect(layout.previewTitleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
    layout.isShown = messageUrl.showPreview();
    layout.descriptionSize = layout.isShown ? documentDescriptionForIndexSize(messageUrl, urlsPreviewWidth) : QSize();
    const QString imagePreviewPath = previewImageUrl.toLocalFile();
    layout.pixmap = mPixmapCache.pixmapForLocalFile(imagePreviewPath);
    layout.pixmap.setDevicePixelRatio(option.widget->devicePixelRatioF());
    const auto dpr = layout.pixmap.devicePixelRatioF();
    layout.imageSize = layout.pixmap.size().scaled(urlsPreviewWidth * dpr, /*imageMaxHeight*/ 100 * dpr, Qt::KeepAspectRatio);

    return layout;
}

QTextDocument *MessageDelegateHelperUrlPreview::documentDescriptionForIndex(const MessageUrl &messageUrl, int width) const
{
    const QString urlId = messageUrl.urlId();
    auto it = mDocumentCache.find(urlId);
    if (it != mDocumentCache.end()) {
        auto ret = it->value.get();
        if (width != -1 && !qFuzzyCompare(ret->textWidth(), width)) {
            ret->setTextWidth(width);
        }
        return ret;
    }

    const QString description = messageUrl.htmlDescription();

    // qDebug() << " description " << description;
    if (description.isEmpty()) {
        return nullptr;
    }
    // Use TextConverter in case it starts with a [](URL) reply marker
    QString needUpdateMessageId; // TODO use it ?
    int maximumRecursiveQuotedText = -1;
    if (mRocketChatAccount) {
        maximumRecursiveQuotedText = mRocketChatAccount->ruqolaServerConfig()->messageQuoteChainLimit();
    }
    const TextConverter::ConvertMessageTextSettings settings(description,
                                                             mRocketChatAccount ? mRocketChatAccount->userName() : QString(),
                                                             {},
                                                             mRocketChatAccount ? mRocketChatAccount->highlightWords() : QStringList(),
                                                             mRocketChatAccount ? mRocketChatAccount->emojiManager() : nullptr,
                                                             mRocketChatAccount ? mRocketChatAccount->messageCache() : nullptr,
                                                             {},
                                                             {},
                                                             {},
                                                             maximumRecursiveQuotedText);

    int recursiveIndex = 0;
    const QString contextString = TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex);
    auto doc = MessageDelegateUtils::createTextDocument(false, contextString, width);
    auto ret = doc.get();
    // qDebug() << " contextString " << contextString;
    mDocumentCache.insert(urlId, std::move(doc));
    return ret;
}

QSize MessageDelegateHelperUrlPreview::documentDescriptionForIndexSize(const MessageUrl &messageUrl, int width) const
{
    auto *doc = documentDescriptionForIndex(messageUrl, width);
    // Add +10 as if we use only doc->idealWidth() it's too small and it creates a new line.
    return doc ? QSize(doc->idealWidth() + 10, doc->size().height()) : QSize();
}

void MessageDelegateHelperUrlPreview::drawDescription(const MessageUrl &messageUrl,
                                                      QRect previewRect,
                                                      QPainter *painter,
                                                      int topPos,
                                                      const QModelIndex &index,
                                                      const QStyleOptionViewItem &option) const
{
    auto *doc = documentDescriptionForIndex(messageUrl, previewRect.width());
    if (!doc) {
        return;
    }

    MessageDelegateUtils::drawSelection(doc, previewRect, topPos, painter, index, option, mTextSelectionImpl->textSelection(), {}, messageUrl);
}

QSize MessageDelegateHelperUrlPreview::sizeHint(const MessageUrl &messageUrl, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    const PreviewLayout layout = layoutPreview(messageUrl, option, maxWidth, -1);
    int height = layout.previewTitleSize.height() + DelegatePaintUtil::margin();
    int pixmapWidth = 0;
    if (layout.isShown) {
        pixmapWidth = qMin(layout.pixmap.width(), maxWidth);
        height += qMin(layout.pixmap.height(), 100) + DelegatePaintUtil::margin();
    }
    int descriptionWidth = 0;
    if (layout.hasDescription && layout.isShown) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(qMax(pixmapWidth, layout.previewTitleSize.width()), descriptionWidth), height};
}

bool MessageDelegateHelperUrlPreview::handleHelpEvent(QHelpEvent *helpEvent,
                                                      QRect previewRect,
                                                      const MessageUrl &messageUrl,
                                                      const QStyleOptionViewItem &option)
{
    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }

    const PreviewLayout layout = layoutPreview(messageUrl, option, previewRect.width(), previewRect.height());

    const auto *doc = documentDescriptionForIndex(messageUrl, previewRect.width());
    if (!doc) {
        return false;
    }

    const QPoint pos =
        helpEvent->pos() - previewRect.topLeft() - QPoint(0, layout.imageSize.height() + layout.previewTitleSize.height() + DelegatePaintUtil::margin());
    QString formattedTooltip;
    if (MessageDelegateUtils::generateToolTip(doc, pos, formattedTooltip)) {
        QToolTip::showText(helpEvent->globalPos(), formattedTooltip);
        return true;
    }
    return true;
}

bool MessageDelegateHelperUrlPreview::handleMouseEvent(const MessageUrl &messageUrl,
                                                       QMouseEvent *mouseEvent,
                                                       QRect previewRect,
                                                       const QStyleOptionViewItem &option,
                                                       const QModelIndex &index)
{
    const QEvent::Type eventType = mouseEvent->type();
    switch (eventType) {
    case QEvent::MouseButtonRelease: {
        const QPoint pos = mouseEvent->pos();
        const PreviewLayout layout = layoutPreview(messageUrl, option, previewRect.width(), previewRect.height());
        if (layout.hideShowButtonRect.translated(previewRect.topLeft()).contains(pos)) {
            MessagesModel::AttachmentAndUrlPreviewVisibility previewUrlVisibility;
            previewUrlVisibility.show = !layout.isShown;
            previewUrlVisibility.ElementId = messageUrl.urlId();
            auto model = const_cast<QAbstractItemModel *>(index.model());
            model->setData(index, QVariant::fromValue(previewUrlVisibility), MessagesModel::DisplayUrlPreview);
            return true;
        }
        // Clicks on links
        auto *doc = documentDescriptionForIndex(messageUrl, previewRect.width());
        if (doc) {
            const QPoint mouseClickPos =
                pos - previewRect.topLeft() - QPoint(0, layout.imageSize.height() + layout.previewTitleSize.height() + DelegatePaintUtil::margin());
            const QString link = doc->documentLayout()->anchorAt(mouseClickPos);
            if (!link.isEmpty()) {
                Q_EMIT mRocketChatAccount->openLinkRequested(link);
                return true;
            }
        }
    }
    default:
        break;
    }
    return false;
}
