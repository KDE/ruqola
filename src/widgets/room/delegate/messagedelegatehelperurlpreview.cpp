/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperurlpreview.h"

#include "common/delegatepaintutil.h"
#include "delegateutils/messagedelegateutils.h"
#include "messages/message.h"
#include "messages/messageurl.h"
#include "rocketchataccount.h"
#include "textconverter.h"

#include <QPainter>
#include <QStyleOptionViewItem>

MessageDelegateHelperUrlPreview::MessageDelegateHelperUrlPreview(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageDelegateHelperUrlPreview::~MessageDelegateHelperUrlPreview() = default;

QTextDocument *MessageDelegateHelperUrlPreview::documentForIndex(const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_ASSERT(false);
    return nullptr;
}

QTextDocument *MessageDelegateHelperUrlPreview::documentForIndex(const MessageAttachment &msgAttach) const
{
    Q_UNUSED(msgAttach);
    Q_ASSERT(false);
    return nullptr;
}

QTextDocument *MessageDelegateHelperUrlPreview::documentForIndex(const Block &block) const
{
    Q_UNUSED(block);
    Q_ASSERT(false);
    return nullptr;
}

void MessageDelegateHelperUrlPreview::draw(const MessageUrl &messageUrl,
                                           QPainter *painter,
                                           QRect previewRect,
                                           const QModelIndex &index,
                                           const QStyleOptionViewItem &option) const
{
    const PreviewLayout layout = layoutPreview(messageUrl, option /*, previewsRect.width(), previewsRect.height()*/);
    painter->drawText(previewRect.x(), previewRect.y() + option.fontMetrics.ascent(), layout.title);

    if (!layout.imageUrl.isEmpty()) {
        qDebug() << " drawIcon " << layout.imageUrl;
        // Draw title and buttons
        const QIcon hideShowIcon = QIcon::fromTheme(layout.isShown ? QStringLiteral("visibility") : QStringLiteral("hint"));
        hideShowIcon.paint(painter, layout.hideShowButtonRect.translated(previewRect.topLeft()));
        // TODO
    }
    // TODO
}

MessageDelegateHelperUrlPreview::PreviewLayout MessageDelegateHelperUrlPreview::layoutPreview(const MessageUrl &messageUrl,
                                                                                              const QStyleOptionViewItem &option) const
{
    MessageDelegateHelperUrlPreview::PreviewLayout layout;
    layout.title = messageUrl.pageTitle();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);

    layout.description = messageUrl.description();
    layout.imageUrl = messageUrl.imageUrl();
    if (!layout.imageUrl.isEmpty()) {
        const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
        layout.hideShowButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
    }
    layout.isShown = messageUrl.showPreview();
    // TODO layout.descriptionSize = documentDescriptionForIndexSize(messageUrl, attachmentsWidth);
    // TODO
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

    const QString description = messageUrl.description();

    if (description.isEmpty()) {
        return nullptr;
    }
    // Use TextConverter in case it starts with a [](URL) reply marker
    QString needUpdateMessageId; // TODO use it ?
    // Laurent Ruqola::self()->rocketChatAccount() only for test.
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
    mDocumentCache.insert(urlId, std::move(doc));
    return ret;
}

QSize MessageDelegateHelperUrlPreview::documentDescriptionForIndexSize(const MessageUrl &messageUrl, int width) const
{
    auto *doc = documentDescriptionForIndex(messageUrl, width);
    // Add +10 as if we use only doc->idealWidth() it's too small and it creates a new line.
    return doc ? QSize(doc->idealWidth() + 10, doc->size().height()) : QSize();
}

QSize MessageDelegateHelperUrlPreview::sizeHint(const MessageUrl &messageUrl, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    const PreviewLayout layout = layoutPreview(messageUrl, option /*, previewsRect.width(), previewsRect.height()*/);
    int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    int pixmapWidth = 0;
    if (layout.isShown) {
        pixmapWidth = qMin(layout.pixmap.width(), maxWidth);
        height += qMin(layout.pixmap.height(), 200) + DelegatePaintUtil::margin();
    }
    int descriptionWidth = 0;
    if (!layout.description.isEmpty()) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(qMax(pixmapWidth, layout.titleSize.width()), descriptionWidth), height};
}

bool MessageDelegateHelperUrlPreview::handleHelpEvent(QHelpEvent *helpEvent,
                                                      QRect previewRect,
                                                      const MessageUrl &messageUrl,
                                                      const QStyleOptionViewItem &option)
{
    if (helpEvent->type() != QEvent::ToolTip) {
        return false;
    }
    // TODO
    return false;
}

bool MessageDelegateHelperUrlPreview::handleMouseEvent(const MessageUrl &msgAttach,
                                                       QMouseEvent *mouseEvent,
                                                       QRect previewRect,
                                                       const QStyleOptionViewItem &option,
                                                       const QModelIndex &index)
{
#if 0
    const QEvent::Type eventType = mouseEvent->type();
    switch (eventType) {
    case QEvent::MouseButtonRelease: {
        const QPoint pos = mouseEvent->pos();

        const VideoLayout layout = layoutVideo(msgAttach, option, attachmentsRect.width());
        if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            MessageAttachmentDownloadAndSaveJob::MessageAttachmentDownloadJobInfo info;
            info.attachmentType = MessageAttachmentDownloadAndSaveJob::AttachmentType::Video;
            info.actionType = MessageAttachmentDownloadAndSaveJob::ActionType::DownloadAndSave;
            info.needToDownloadAttachment = !mRocketChatAccount->attachmentIsInLocalCache(layout.videoPath);
            info.parentWidget = const_cast<QWidget *>(option.widget);
            info.attachmentPath = layout.videoPath;
            auto job = new MessageAttachmentDownloadAndSaveJob(this);
            job->setRocketChatAccount(mRocketChatAccount);
            job->setInfo(info);
            job->start();
            return true;
        } else if (QRect(attachmentsRect.topLeft(), layout.titleSize).contains(pos)
                   || layout.showButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            auto parentWidget = const_cast<QWidget *>(option.widget);
            ShowVideoDialog dlg(mRocketChatAccount, parentWidget);
            dlg.setVideoPath(layout.videoPath);
            dlg.exec();
            return true;
        }
        break;
    }
    default:
        break;
    }
#endif
    // TODO
    return false;
}
