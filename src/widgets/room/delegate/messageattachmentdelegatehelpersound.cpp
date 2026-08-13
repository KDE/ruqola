/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelpersound.h"

#include "common/delegatepaintutil.h"
#include "dialogs/playsounddialog.h"
#include "misc/messageattachmentdownloadandsavejob.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

using namespace Qt::Literals::StringLiterals;
MessageAttachmentDelegateHelperSound::MessageAttachmentDelegateHelperSound(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageAttachmentDelegateHelperBase(account, view, textSelectionImpl)
    , mPlayerVolumeIcon(QIcon::fromTheme(u"player-volume"_s))
    , mDownloadIcon(QIcon::fromTheme(u"cloud-download"_s))
{
}

MessageAttachmentDelegateHelperSound::~MessageAttachmentDelegateHelperSound() = default;

void MessageAttachmentDelegateHelperSound::draw(const MessageAttachment &msgAttach,
                                                QPainter *painter,
                                                QRect messageRect,
                                                const QModelIndex &index,
                                                const QStyleOptionViewItem &option) const
{
    const SoundLayout layout = layoutSound(msgAttach, option, messageRect.width());
    // drawTitle(msgAttach, painter, );
    // Draw title and buttons
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);
    mPlayerVolumeIcon.paint(painter, layout.playerVolumeButtonRect.translated(messageRect.topLeft()));
    if (layout.downloadButtonRect.isValid()) {
        mDownloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));
    }

    const int nextY = messageRect.y() + layout.headerSize.height() + DelegatePaintUtil::margin();
    drawDescription(msgAttach, messageRect, painter, nextY, index, option);
}

QSize MessageAttachmentDelegateHelperSound::sizeHint(const MessageAttachment &msgAttach,
                                                     [[maybe_unused]] const QModelIndex &index,
                                                     int maxWidth,
                                                     const QStyleOptionViewItem &option) const
{
    const SoundLayout layout = layoutSound(msgAttach, option, maxWidth);
    int height = layout.headerSize.height() + DelegatePaintUtil::margin();
    int descriptionWidth = 0;
    if (layout.hasDescription) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(layout.headerSize.width(), descriptionWidth), height};
}

QPoint MessageAttachmentDelegateHelperSound::adaptMousePosition(const QPoint &pos,
                                                                const MessageAttachment &msgAttach,
                                                                QRect attachmentsRect,
                                                                const QStyleOptionViewItem &option)
{
    const SoundLayout layout = layoutSound(msgAttach, option, attachmentsRect.width());
    const QPoint relativePos = pos - attachmentsRect.topLeft() - QPoint(0, layout.headerSize.height() + DelegatePaintUtil::margin());
    return relativePos;
}

bool MessageAttachmentDelegateHelperSound::handleMouseEvent(const MessageAttachment &msgAttach,
                                                            QMouseEvent *mouseEvent,
                                                            QRect attachmentsRect,
                                                            const QStyleOptionViewItem &option,
                                                            const QModelIndex &index)
{
    const QEvent::Type eventType = mouseEvent->type();
    switch (eventType) {
    case QEvent::MouseButtonRelease: {
        // Only a plain left click acts on the buttons: a release ending a text selection must go to the
        // base class, otherwise selecting the description pops up the sound dialog.
        if (mouseEvent->button() != Qt::LeftButton || mTextSelectionImpl->textSelection()->hasSelection()) {
            break;
        }
        const QPoint pos = mouseEvent->pos();

        // downloadButtonRect is invalid when the attachment can't be downloaded => it contains() nothing
        const SoundLayout layout = layoutSound(msgAttach, option, attachmentsRect.width());
        if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            MessageAttachmentDownloadAndSaveJob::MessageAttachmentDownloadJobInfo info;
            info.attachmentType = MessageAttachmentDownloadAndSaveJob::AttachmentType::Sound;
            info.actionType = MessageAttachmentDownloadAndSaveJob::ActionType::DownloadAndSave;
            info.attachmentPath = layout.audioPath;
            info.needToDownloadAttachment = !mRocketChatAccount->attachmentIsInLocalCache(info.attachmentPath);
            info.parentWidget = const_cast<QWidget *>(option.widget);
            auto job = new MessageAttachmentDownloadAndSaveJob(this);
            job->setRocketChatAccount(mRocketChatAccount);
            job->setInfo(info);
            job->start();
            return true;
        } else if (QRect(attachmentsRect.topLeft(), layout.titleSize).contains(pos)
                   || layout.playerVolumeButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            auto parentWidget = const_cast<QWidget *>(option.widget);
            PlaySoundDialog dlg(mRocketChatAccount, parentWidget);
            dlg.setAudioPath(layout.audioPath);
            dlg.exec();
            return true;
        }
        break;
    }
    default:
        break;
    }

    return MessageAttachmentDelegateHelperBase::handleMouseEvent(msgAttach, mouseEvent, attachmentsRect, option, index);
}

MessageAttachmentDelegateHelperSound::SoundLayout
MessageAttachmentDelegateHelperSound::layoutSound(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const
{
    SoundLayout layout;
    // or we could do layout.attachment = msgAttach; if we need many fields from it
    layout.title = msgAttach.attachmentGeneratedTitle();
    layout.hasDescription = msgAttach.hasDescription();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.descriptionSize = documentTypeForIndexSize(convertAttachmentToDocumentDescriptionInfo(msgAttach, attachmentsWidth));
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    const int headerHeight = qMax(layout.titleSize.height(), iconSize);
    const int iconY = (headerHeight - iconSize) / 2;
    layout.playerVolumeButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), iconY, iconSize, iconSize);
    int headerWidth = layout.playerVolumeButtonRect.right() + 1;
    if (msgAttach.canDownloadAttachment()) {
        layout.downloadButtonRect = layout.playerVolumeButtonRect.translated(iconSize + DelegatePaintUtil::margin(), 0);
        headerWidth = layout.downloadButtonRect.right() + 1;
    }
    layout.headerSize = QSize(headerWidth, headerHeight);
    layout.audioPath = msgAttach.link();
    return layout;
}
