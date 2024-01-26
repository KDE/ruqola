/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperurlpreview.h"

#include "common/delegatepaintutil.h"
#include "delegateutils/messagedelegateutils.h"
#include "messages/messageurl.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_selection_debug.h"

#include <KLocalizedString>

#include <QDrag>
#include <QListView>
#include <QMimeData>
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
    // italicFont.setBold(true);
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
            // qDebug() << " image size " << scaledPixmap.size();
            nextY += scaledPixmap.height() / scaledPixmap.devicePixelRatioF() + DelegatePaintUtil::margin();
        }
        // qDebug() << " nextY " << nextY;
#if 0
        painter->save();
        painter->setPen(Qt::red);
        painter->drawRect(previewRect);
        painter->restore();
#endif
        drawDescription(messageUrl, previewRect, painter, nextY, index, option);
    }
}

MessageDelegateHelperUrlPreview::PreviewLayout MessageDelegateHelperUrlPreview::layoutPreview(const MessageUrl &messageUrl,
                                                                                              const QStyleOptionViewItem &option,
                                                                                              int urlsPreviewWidth,
                                                                                              int urlsPreviewHeight) const
{
    Q_UNUSED(urlsPreviewHeight);
    MessageDelegateHelperUrlPreview::PreviewLayout layout;
    layout.previewTitle = i18n("Link Preview");
    layout.previewTitleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.previewTitle);
    layout.hasDescription = messageUrl.hasHtmlDescription();
    const QUrl previewImageUrl =
        messageUrl.imageUrl().isEmpty() ? QUrl{} : (mRocketChatAccount ? mRocketChatAccount->previewUrlFromLocalCache(messageUrl.imageUrl()) : QUrl{});
    if (previewImageUrl.isLocalFile()) {
        layout.imageUrl = messageUrl.imageUrl();

        const QString imagePreviewPath = previewImageUrl.toLocalFile();
        layout.pixmap = mPixmapCache.pixmapForLocalFile(imagePreviewPath);
        layout.pixmap.setDevicePixelRatio(option.widget->devicePixelRatioF());
        const auto dpr = layout.pixmap.devicePixelRatioF();
        layout.imageSize = layout.pixmap.size().scaled(urlsPreviewWidth * dpr, /*imageMaxHeight*/ 100 * dpr, Qt::KeepAspectRatio);
        // qDebug() << " layout.imageSize " << layout.imageSize;
    }
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    layout.hideShowButtonRect = QRect(layout.previewTitleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
    layout.isShown = messageUrl.showPreview();
    layout.descriptionSize =
        layout.isShown ? documentDescriptionForIndexSize(convertMessageUrlToDocumentDescriptionInfo(messageUrl, urlsPreviewWidth)) : QSize();

    return layout;
}

MessageDelegateHelperBase::DocumentDescriptionInfo MessageDelegateHelperUrlPreview::convertMessageUrlToDocumentDescriptionInfo(const MessageUrl &messageUrl,
                                                                                                                               int width) const
{
    MessageDelegateHelperBase::DocumentDescriptionInfo info;
    info.documentId = messageUrl.urlId();
    info.description = messageUrl.htmlDescription();
    info.width = width;
    return info;
}

void MessageDelegateHelperUrlPreview::drawDescription(const MessageUrl &messageUrl,
                                                      QRect previewRect,
                                                      QPainter *painter,
                                                      int topPos,
                                                      const QModelIndex &index,
                                                      const QStyleOptionViewItem &option) const
{
    auto *doc = documentDescriptionForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, previewRect.width()));
    if (!doc) {
        return;
    }

    MessageDelegateUtils::drawSelection(doc, previewRect, topPos, painter, index, option, mTextSelectionImpl->textSelection(), {}, messageUrl);
}

QSize MessageDelegateHelperUrlPreview::sizeHint(const MessageUrl &messageUrl, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index);
    const PreviewLayout layout = layoutPreview(messageUrl, option, maxWidth, -1);
    int height = layout.previewTitleSize.height() + DelegatePaintUtil::margin();
    // qDebug() << " height 1 " << height;
    int pixmapWidth = 0;
    if (layout.isShown) {
        pixmapWidth = qMin(layout.pixmap.width(), maxWidth);
        height += qMin(layout.imageSize.height(), 100) + DelegatePaintUtil::margin();
        // qDebug() << " height 2 " << height << "  layout.pixmap.height() " << layout.imageSize.height();
    }
    int descriptionWidth = 0;
    if (layout.hasDescription && layout.isShown) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
        // qDebug() << " height 3 " << height;
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

    const auto *doc = documentDescriptionForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, previewRect.width()));
    if (!doc) {
        return false;
    }
    const PreviewLayout layout = layoutPreview(messageUrl, option, previewRect.width(), previewRect.height());

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
    const QPoint pos = mouseEvent->pos();
    switch (eventType) {
    case QEvent::MouseButtonRelease: {
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
        auto *doc = documentDescriptionForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, previewRect.width()));
        if (doc) {
            const QPoint mouseClickPos =
                pos - previewRect.topLeft() - QPoint(0, layout.imageSize.height() + layout.previewTitleSize.height() + DelegatePaintUtil::margin());
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
        if (const auto *doc = documentDescriptionForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, previewRect.width()))) {
            const int charPos = charPosition(doc, messageUrl, previewRect, pos, option);
            qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "pressed at pos" << charPos;
            if (charPos == -1) {
                return false;
            }
            if (mTextSelectionImpl->textSelection()->contains(index, charPos) && doc->documentLayout()->hitTest(pos, Qt::ExactHit) != -1) {
                mTextSelectionImpl->setMightStartDrag(true);
                return true;
            }

            // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
            // (look there if you want to add these things)

            mTextSelectionImpl->textSelection()->setStart(index, charPos);
            return true;
        } else {
            mTextSelectionImpl->textSelection()->clear();
        }
        break;
    case QEvent::MouseMove:
        if (!mTextSelectionImpl->mightStartDrag()) {
            if (const auto *doc = documentDescriptionForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, previewRect.width()))) {
                const int charPos = charPosition(doc, messageUrl, previewRect, pos, option);
                if (charPos != -1) {
                    // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                    mTextSelectionImpl->textSelection()->setEnd(index, charPos);
                    return true;
                }
            }
        }
        break;
    case QEvent::MouseButtonDblClick:
        if (!mTextSelectionImpl->textSelection()->hasSelection()) {
            if (const auto *doc = documentDescriptionForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, previewRect.width()))) {
                const int charPos = charPosition(doc, messageUrl, previewRect, pos, option);
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

int MessageDelegateHelperUrlPreview::charPosition(const QTextDocument *doc,
                                                  const MessageUrl &messageUrl,
                                                  QRect previewRect,
                                                  const QPoint &pos,
                                                  const QStyleOptionViewItem &option)
{
    const QPoint relativePos = adaptMousePosition(pos, messageUrl, previewRect, option);
    const int charPos = doc->documentLayout()->hitTest(relativePos, Qt::FuzzyHit);
    return charPos;
}

QPoint
MessageDelegateHelperUrlPreview::adaptMousePosition(const QPoint &pos, const MessageUrl &messageUrl, QRect previewRect, const QStyleOptionViewItem &option)
{
    const PreviewLayout layout = layoutPreview(messageUrl, option, previewRect.width(), previewRect.height());
    const QPoint relativePos =
        pos - previewRect.topLeft() - QPoint(0, layout.imageSize.height() + layout.previewTitleSize.height() + DelegatePaintUtil::margin());
    return relativePos;
}

QString MessageDelegateHelperUrlPreview::urlAt(const QStyleOptionViewItem &option, const MessageUrl &messageUrl, QRect previewsRect, QPoint pos)
{
    auto document = documentDescriptionForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, previewsRect.width()));
    if (!document) {
        return {};
    }
    const QPoint relativePos = adaptMousePosition(pos, messageUrl, previewsRect, option);
    return document->documentLayout()->anchorAt(relativePos);
}

QTextDocument *MessageDelegateHelperUrlPreview::documentForUrlPreview(const MessageUrl &messageUrl) const
{
    return documentDescriptionForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, -1));
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
        const auto *doc = documentDescriptionForIndex(convertMessageUrlToDocumentDescriptionInfo(messageUrl, previewsRect.width()));
        const QPoint pos = mouseEvent->pos() - previewsRect.topLeft();
        const int charPos = doc->documentLayout()->hitTest(pos, Qt::FuzzyHit);
        if (charPos != -1 && mTextSelectionImpl->textSelection()->contains(index, charPos)) {
            auto mimeData = new QMimeData;
            mimeData->setHtml(mTextSelectionImpl->textSelection()->selectedText(TextSelection::Html));
            mimeData->setText(mTextSelectionImpl->textSelection()->selectedText(TextSelection::Text));
            auto drag = new QDrag(const_cast<QWidget *>(option.widget));
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction);
            mTextSelectionImpl->setMightStartDrag(false); // don't clear selection on release
            return true;
        }
    }
    return false;
}

void MessageDelegateHelperUrlPreview::dump(const PreviewLayout &layout)
{
    // Don't use debug category as we want to show it.
    qDebug() << " pixmap " << layout.pixmap;
    qDebug() << " imageUrl " << layout.imageUrl;
    qDebug() << " hasDescription " << layout.hasDescription;
    qDebug() << " previewTitleSize " << layout.previewTitleSize;
    qDebug() << " previewTitle " << layout.previewTitle;
    qDebug() << " descriptionSize " << layout.descriptionSize;
    qDebug() << " imageSize " << layout.imageSize;
    qDebug() << " hideShowButtonRect " << layout.hideShowButtonRect;
    qDebug() << " isShown " << layout.isShown;
}
