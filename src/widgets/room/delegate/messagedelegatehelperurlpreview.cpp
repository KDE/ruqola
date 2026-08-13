/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperurlpreview.h"

#include "common/delegatepaintutil.h"
#include "delegateutils/messagedelegateutils.h"
#include "messages/messageurl.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_selection_debug.h"

#include <QDrag>
#include <QListView>
#include <QMimeData>
#include <QPainter>
#include <QPalette>
#include <QStyleOptionViewItem>
#include <QToolTip>

using namespace Qt::Literals::StringLiterals;

namespace
{
// Maximum edge length (logical px) of the thumbnail for a regular link preview:
// small enough that the card reads as a compact card, not a banner.
constexpr int PreviewThumbnailMaxEdge = 64;
// Larger cap for links whose content is itself an image/video.
constexpr int PreviewMediaMaxEdge = 320;
// Inner padding (logical px) between the card's rounded background and its
// content, and the corner radius of that background (matching the card styling
// used by the conference/section delegate helpers).
constexpr int PreviewPadding = 6;
constexpr int PreviewCornerRadius = 5;
// Transparent gap (logical px) above the card, separating it from the message
// text line. Reported as part of the preview's sizeHint height so the message
// layout leaves room for it without any change to the stacking code.
constexpr int PreviewTopGap = 6;
}

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

    // A subtle rounded background sets the preview apart as embedded, secondary
    // content (the job the old "Link Preview" label did, without the chrome).
    // Colours come from the palette so it themes itself for light and dark.
    // The card fills the rect the delegate allocated from sizeHint(), not the freshly
    // recomputed contentWidth: re-running layoutPreview() here can differ by a pixel
    // (QTextDocument::idealWidth() is not perfectly stable across text widths), so using
    // the allocated rect keeps the card within its slot instead of aborting on that drift.
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    QColor borderColor = option.palette.color(QPalette::WindowText);
    borderColor.setAlpha(30);
    painter->setPen(borderColor);
    painter->setBrush(option.palette.color(QPalette::AlternateBase));
    const QRectF cardRect(previewRect.x() + 0.5, previewRect.y() + PreviewTopGap + 0.5, previewRect.width() - 1, previewRect.height() - PreviewTopGap - 1);
    painter->drawRoundedRect(cardRect, PreviewCornerRadius, PreviewCornerRadius);
    painter->restore();

    const QPoint contentTopLeft = previewRect.topLeft() + QPoint(PreviewPadding, PreviewTopGap + PreviewPadding);

    // A subtle collapse/expand affordance, right-aligned. A chevron reads as
    // "there is more/less to see" without the eye icon's visual weight; fall
    // back to the previous icons should the theme lack the chevrons.
    QIcon toggleIcon = QIcon::fromTheme(layout.isShown ? u"go-up"_s : u"go-down"_s);
    if (toggleIcon.isNull()) {
        toggleIcon = QIcon::fromTheme(layout.isShown ? u"visibility"_s : u"hint"_s);
    }
    toggleIcon.paint(painter, layout.hideShowButtonRect.translated(previewRect.topLeft()));

    if (!layout.isShown) {
        // Collapsed: just the page title on one line, drawn as a clickable link
        // so the preview still tells you where it goes.
        painter->save();
        painter->setPen(option.palette.color(QPalette::Link));
        painter->drawText(contentTopLeft.x(), contentTopLeft.y() + option.fontMetrics.ascent(), layout.collapsedTitle);
        painter->restore();
        return;
    }

    if (!layout.pixmap.isNull()) {
        QPixmap scaledPixmap = layout.pixmap.scaled(layout.imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        scaledPixmap.setDevicePixelRatio(layout.pixmap.devicePixelRatioF());
        painter->drawPixmap(contentTopLeft, scaledPixmap);
    }
    drawDescription(messageUrl, previewRect, painter, index, option, layout);
}

MessageDelegateHelperUrlPreview::PreviewLayout MessageDelegateHelperUrlPreview::layoutPreview(const MessageUrl &messageUrl,
                                                                                              const QStyleOptionViewItem &option,
                                                                                              int urlsPreviewWidth,
                                                                                              [[maybe_unused]] int urlsPreviewHeight) const
{
    MessageDelegateHelperUrlPreview::PreviewLayout layout;
    layout.isShown = messageUrl.showPreview();

    const int margin = DelegatePaintUtil::margin();
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    // Space kept free on the right for the collapse/expand toggle (gap + icon).
    const int rightReserve = margin + iconSize;

    if (!layout.isShown) {
        // Collapsed card: one compact line with the (elided) page title.
        const int titleAvailableWidth = qMax(0, urlsPreviewWidth - 2 * PreviewPadding - rightReserve);
        const QString title = messageUrl.pageTitle().isEmpty() ? messageUrl.url() : messageUrl.pageTitle();
        layout.collapsedTitle = option.fontMetrics.elidedText(title, Qt::ElideRight, titleAvailableWidth);
        layout.collapsedTitleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.collapsedTitle);
        layout.collapsedTitleRect = QRect(QPoint(PreviewPadding, PreviewTopGap + PreviewPadding), layout.collapsedTitleSize);
        const int innerWidth = layout.collapsedTitleSize.width() + rightReserve;
        const int innerHeight = qMax(layout.collapsedTitleSize.height(), iconSize);
        layout.contentWidth = innerWidth + 2 * PreviewPadding;
        layout.contentHeight = PreviewTopGap + innerHeight + 2 * PreviewPadding;
        layout.hideShowButtonRect = QRect(layout.contentWidth - PreviewPadding - iconSize, PreviewTopGap + PreviewPadding, iconSize, iconSize);
        return layout;
    }

    // Expanded card: [ thumbnail ] [ title + description ] [ toggle ]
    QUrl previewImageUrl;
    if (messageUrl.imageUrl().isEmpty() || messageUrl.hasPreviewUrl()) {
        previewImageUrl = mRocketChatAccount ? mRocketChatAccount->previewUrlFromLocalCache(messageUrl.buildImageUrl()) : QUrl{};
    }
    if (previewImageUrl.isLocalFile()) {
        layout.imageUrl = messageUrl.imageUrl();

        const QString imagePreviewPath = previewImageUrl.toLocalFile();
        layout.pixmap = mPixmapCache.pixmapForLocalFile(imagePreviewPath);
        layout.pixmap.setDevicePixelRatio(option.widget->devicePixelRatioF());
        const auto dpr = layout.pixmap.devicePixelRatioF();
        // Regular link previews get a small, favicon-like thumbnail; links whose
        // content *is* an image/video keep a larger picture since that is the
        // point. Never upscale past the image's own size.
        const bool mediaContent = messageUrl.contentType() != MessageUrl::ContentType::None;
        const int maxEdge = mediaContent ? PreviewMediaMaxEdge : PreviewThumbnailMaxEdge;
        const int maxEdgeDevice = qRound(maxEdge * dpr);
        layout.imageSize = layout.pixmap.size().scaled(maxEdgeDevice, maxEdgeDevice, Qt::KeepAspectRatio).boundedTo(layout.pixmap.size());
    }

    const qreal dpr = layout.pixmap.isNull() ? 1.0 : layout.pixmap.devicePixelRatioF();
    const QSize thumbLogical = layout.imageSize.isEmpty() ? QSize() : QSize(qRound(layout.imageSize.width() / dpr), qRound(layout.imageSize.height() / dpr));
    const int leftReserve = thumbLogical.isEmpty() ? 0 : (thumbLogical.width() + margin);

    layout.textLeftOffset = leftReserve;
    layout.docWidth = qMax(0, urlsPreviewWidth - 2 * PreviewPadding - leftReserve - rightReserve);
    layout.descriptionSize = documentTypeForIndexSize(convertMessageUrlToDocumentDescriptionInfo(messageUrl, layout.docWidth));

    // contentWidth adds the padding and reserves back onto docWidth, so it is the exact
    // inverse of the docWidth computation above. sizeHint() reports it as the slot width;
    // draw() then works from the slot rect it is handed rather than re-deriving it.
    const int innerWidth = leftReserve + layout.descriptionSize.width() + rightReserve;
    const int innerHeight = qMax(qMax(thumbLogical.height(), layout.descriptionSize.height()), iconSize);
    layout.contentWidth = innerWidth + 2 * PreviewPadding;
    layout.contentHeight = PreviewTopGap + innerHeight + 2 * PreviewPadding;
    layout.hideShowButtonRect = QRect(layout.contentWidth - PreviewPadding - iconSize, PreviewTopGap + PreviewPadding, iconSize, iconSize);
    return layout;
}

MessageDelegateHelperBase::DocumentTypeInfo MessageDelegateHelperUrlPreview::convertMessageUrlToDocumentDescriptionInfo(const MessageUrl &messageUrl,
                                                                                                                        int width) const
{
    MessageDelegateHelperBase::DocumentTypeInfo info;
    info.identifier = messageUrl.urlId();
    // The selectable description document only exists while the preview is
    // expanded; when collapsed we paint the title ourselves, so return empty
    // text (which yields a null document) to keep a single cache key per url.
    info.text = messageUrl.showPreview() ? messageUrl.htmlDescription() : QString();
    info.width = width;
    return info;
}

void MessageDelegateHelperUrlPreview::drawDescription(const MessageUrl &messageUrl,
                                                      QRect previewRect,
                                                      QPainter *painter,
                                                      const QModelIndex &index,
                                                      const QStyleOptionViewItem &option,
                                                      const PreviewLayout &layout) const
{
    auto *doc = documentTypeForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, layout.docWidth));
    if (!doc) {
        return;
    }

    const QRect textRect(previewRect.x() + PreviewPadding + layout.textLeftOffset,
                         previewRect.y() + PreviewTopGap + PreviewPadding,
                         layout.docWidth,
                         layout.descriptionSize.height());
    MessageDelegateUtils::drawSelection(doc, textRect, textRect.top(), painter, index, option, mTextSelectionImpl->textSelection(), {}, messageUrl);
}

QSize MessageDelegateHelperUrlPreview::sizeHint(const MessageUrl &messageUrl,
                                                [[maybe_unused]] const QModelIndex &index,
                                                int maxWidth,
                                                const QStyleOptionViewItem &option) const
{
    const PreviewLayout layout = layoutPreview(messageUrl, option, maxWidth, -1);
    return {layout.contentWidth, layout.contentHeight};
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
    const auto *doc = documentTypeForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, layout.docWidth));
    if (!doc) {
        return false;
    }
    const QPoint pos = relativePos(helpEvent->pos(), layout, previewRect);
    QString formattedTooltip;
    if (MessageDelegateUtils::generateToolTip(doc, pos, formattedTooltip)) {
        QToolTip::showText(helpEvent->globalPos(), formattedTooltip);
        return true;
    }
    return false;
}

bool MessageDelegateHelperUrlPreview::handleMouseEvent(const MessageUrl &messageUrl,
                                                       QMouseEvent *mouseEvent,
                                                       QRect previewRect,
                                                       const QStyleOptionViewItem &option,
                                                       const QModelIndex &index)
{
    const QEvent::Type eventType = mouseEvent->type();
    const QPoint pos = mouseEvent->pos();
    const PreviewLayout layout = layoutPreview(messageUrl, option, previewRect.width(), previewRect.height());
    switch (eventType) {
    case QEvent::MouseButtonRelease: {
        if (layout.hideShowButtonRect.translated(previewRect.topLeft()).contains(pos)) {
            MessagesModel::AttachmentAndUrlPreviewVisibility previewUrlVisibility;
            previewUrlVisibility.show = !layout.isShown;
            previewUrlVisibility.elementId = messageUrl.urlId();
            auto model = const_cast<QAbstractItemModel *>(index.model());
            model->setData(index, QVariant::fromValue(previewUrlVisibility), MessagesModel::DisplayUrlPreview);
            return true;
        }
        if (!layout.isShown) {
            // Clicking the collapsed title opens the link.
            if (layout.collapsedTitleRect.translated(previewRect.topLeft()).contains(pos)) {
                Q_EMIT mRocketChatAccount->openLinkRequested(messageUrl.url());
                return true;
            }
            break;
        }
        // Clicks on links inside the description
        if (const auto *doc = documentTypeForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, layout.docWidth))) {
            const QPoint mouseClickPos = relativePos(pos, layout, previewRect);
            const QString link = doc->documentLayout()->anchorAt(mouseClickPos);
            if (!link.isEmpty()) {
                Q_EMIT mRocketChatAccount->openLinkRequested(link);
                return true;
            }
        }
        break;
    }
    case QEvent::MouseButtonPress:
        mTextSelectionImpl->setMightStartDrag(false);
        if (layout.isShown) {
            if (const auto *doc = documentTypeForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, layout.docWidth))) {
                const int charPos = charPosition(doc, layout, previewRect, pos);
                qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "pressed at pos" << charPos;
                if (charPos == -1) {
                    return false;
                }
                if (mTextSelectionImpl->textSelection()->contains(index, charPos)
                    && doc->documentLayout()->hitTest(relativePos(pos, layout, previewRect), Qt::ExactHit) != -1) {
                    mTextSelectionImpl->setMightStartDrag(true);
                    return true;
                }

                // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
                // (look there if you want to add these things)

                mTextSelectionImpl->textSelection()->setPreviewUrlTextSelectionStart(index, charPos, messageUrl);
                return true;
            }
        }
        mTextSelectionImpl->textSelection()->clear();
        break;
    case QEvent::MouseMove:
        if (layout.isShown && !mTextSelectionImpl->mightStartDrag()) {
            if (const auto *doc = documentTypeForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, layout.docWidth))) {
                const int charPos = charPosition(doc, layout, previewRect, pos);
                if (charPos != -1) {
                    // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                    mTextSelectionImpl->textSelection()->setPreviewUrlTextSelectionEnd(index, charPos, messageUrl);
                    return true;
                }
            }
        }
        break;
    case QEvent::MouseButtonDblClick:
        if (layout.isShown && !mTextSelectionImpl->textSelection()->hasSelection()) {
            if (const auto *doc = documentTypeForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, layout.docWidth))) {
                const int charPos = charPosition(doc, layout, previewRect, pos);
                qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "double-clicked at pos" << charPos;
                if (charPos == -1) {
                    return false;
                }
                mTextSelectionImpl->textSelection()->selectWordUnderCursor(index, charPos, this, messageUrl);
                return true;
            }
        }
        break;
    default:
        break;
    }
    return false;
}

int MessageDelegateHelperUrlPreview::charPosition(const QTextDocument *doc, const PreviewLayout &layout, QRect previewRect, const QPoint &pos)
{
    return doc->documentLayout()->hitTest(relativePos(pos, layout, previewRect), Qt::FuzzyHit);
}

QPoint MessageDelegateHelperUrlPreview::relativePos(const QPoint &pos, const PreviewLayout &layout, QRect previewRect) const
{
    // The description document is laid out inside the card padding (below the
    // top gap), to the right of the thumbnail; translate mouse coordinates into
    // the document's frame.
    return pos - previewRect.topLeft() - QPoint(PreviewPadding + layout.textLeftOffset, PreviewTopGap + PreviewPadding);
}

QString MessageDelegateHelperUrlPreview::urlAt(const QStyleOptionViewItem &option, const MessageUrl &messageUrl, QRect previewsRect, QPoint pos)
{
    const PreviewLayout layout = layoutPreview(messageUrl, option, previewsRect.width(), previewsRect.height());
    auto document = documentTypeForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, layout.docWidth));
    if (!document) {
        return {};
    }
    return document->documentLayout()->anchorAt(relativePos(pos, layout, previewsRect));
}

QTextDocument *MessageDelegateHelperUrlPreview::documentForUrlPreview(const MessageUrl &messageUrl) const
{
    // Gate on the same predicate that decides whether a card is laid out and drawn
    // (see MessageListDelegate::paint / generateAttachmentBlockAndUrlPreviewLayout):
    // a URL without a rich preview occupies no rect and paints nothing, so the
    // selection layer must not hand out a document for it either. When collapsed we
    // paint the title ourselves, so there is likewise no selectable document.
    if (!messageUrl.hasRichPreview() || !messageUrl.showPreview()) {
        return nullptr;
    }
    return documentTypeForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, -1));
}

bool MessageDelegateHelperUrlPreview::maybeStartDrag(const MessageUrl &messageUrl,
                                                     QMouseEvent *mouseEvent,
                                                     QRect previewsRect,
                                                     const QStyleOptionViewItem &option,
                                                     const QModelIndex &index)
{
    if (!mTextSelectionImpl->mightStartDrag() || !previewsRect.contains(mouseEvent->pos())) {
        return false;
    }
    if (mTextSelectionImpl->textSelection()->hasSelection()) {
        const PreviewLayout layout = layoutPreview(messageUrl, option, previewsRect.width(), previewsRect.height());
        if (const auto *doc = documentTypeForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, layout.docWidth))) {
            const QPoint pos = relativePos(mouseEvent->pos(), layout, previewsRect);
            const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
            if (charPos != -1 && mTextSelectionImpl->textSelection()->contains(index, charPos)) {
                auto mimeData = new QMimeData;
                mimeData->setHtml(mTextSelectionImpl->textSelection()->selectedText(TextSelection::Format::Html));
                mimeData->setText(mTextSelectionImpl->textSelection()->selectedText(TextSelection::Format::Text));
                auto drag = new QDrag(const_cast<QWidget *>(option.widget));
                drag->setMimeData(mimeData);
                drag->exec(Qt::CopyAction);
                mTextSelectionImpl->setMightStartDrag(false); // don't clear selection on release
                return true;
            }
        }
    }
    return false;
}

void MessageDelegateHelperUrlPreview::dump(const PreviewLayout &layout)
{
    // Don't use debug category as we want to show it.
    qDebug() << " pixmap " << layout.pixmap;
    qDebug() << " imageUrl " << layout.imageUrl;
    qDebug() << " collapsedTitle " << layout.collapsedTitle;
    qDebug() << " collapsedTitleSize " << layout.collapsedTitleSize;
    qDebug() << " descriptionSize " << layout.descriptionSize;
    qDebug() << " imageSize " << layout.imageSize;
    qDebug() << " textLeftOffset " << layout.textLeftOffset;
    qDebug() << " docWidth " << layout.docWidth;
    qDebug() << " contentWidth " << layout.contentWidth;
    qDebug() << " contentHeight " << layout.contentHeight;
    qDebug() << " hideShowButtonRect " << layout.hideShowButtonRect;
    qDebug() << " isShown " << layout.isShown;
}
