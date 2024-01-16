/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistlayoutbase.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "room/delegate/messagelistdelegate.h"

#include "room/delegate/messageattachmentdelegatehelperbase.h"
#include "room/delegate/messageblockdelegatehelperbase.h"
#include "room/delegate/messagedelegatehelperreactions.h"
#include "room/delegate/messagedelegatehelpertext.h"
#include "room/delegate/messagedelegatehelperurlpreview.h"
#include "room/delegate/messagelistdelegate.h"

MessageListLayoutBase::MessageListLayoutBase(MessageListDelegate *delegate)
    : mDelegate(delegate)
{
}

MessageListLayoutBase::~MessageListLayoutBase() = default;

RocketChatAccount *MessageListLayoutBase::rocketChatAccount() const
{
    return mRocketChatAccount;
}

void MessageListLayoutBase::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}

bool MessageListLayoutBase::sameSenderAsPreviousMessage(const QModelIndex &index, const Message *message) const
{
    if (index.row() < 1) {
        return false;
    }

    const auto previousIndex = index.siblingAtRow(index.row() - 1);
    const auto previousMessage = previousIndex.data(MessagesModel::MessagePointer).value<Message *>();
    Q_ASSERT(previousMessage);

    const int diffDate = mRocketChatAccount ? mRocketChatAccount->ruqolaServerConfig()->messageGroupingPeriod() * 1000 : 0;
    if ((message->userId() == previousMessage->userId()) && (message->threadMessageId() == previousMessage->threadMessageId())
        && (message->timeStamp() <= (previousMessage->timeStamp() + diffDate)))
        return true;
    return false;
}

QString MessageListLayoutBase::senderText(const Message *message) const
{
    if (mRocketChatAccount) {
        return QLatin1Char('@') + (mRocketChatAccount->useRealName() && !message->name().isEmpty() ? message->name() : message->username());
    } else {
        return QLatin1Char('@') + message->username();
    }
}

void MessageListLayoutBase::generateSenderInfo(Layout &layout, const Message *message, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    layout.senderText = senderText(message);
    layout.senderFont = option.font;
    layout.senderFont.setBold(true);
    layout.sameSenderAsPreviousMessage = sameSenderAsPreviousMessage(index, message);
}

void MessageListLayoutBase::generateAttachmentBlockAndUrlPreviewLayout(MessageListDelegate *delegate,
                                                                       Layout &layout,
                                                                       const Message *message,
                                                                       int attachmentsY,
                                                                       int textLeft,
                                                                       int maxWidth,
                                                                       const QStyleOptionViewItem &option,
                                                                       const QModelIndex &index) const
{
    if (message->attachments().isEmpty() && message->blocks().isEmpty() && message->urls().isEmpty()) {
        layout.reactionsY = attachmentsY;
    } else {
        int topAttachment = attachmentsY;
        if (!message->attachments().isEmpty()) {
            const auto attachments = message->attachments();
            QSize attachmentsSize;
            // TODO add spacing between attachment
            for (const MessageAttachment &msgAttach : attachments) {
                const MessageAttachmentDelegateHelperBase *helper = delegate->attachmentsHelper(msgAttach);
                if (attachmentsSize.isEmpty()) {
                    attachmentsSize = helper ? helper->sizeHint(msgAttach, index, maxWidth, option) : QSize(0, 0);
                    layout.attachmentsRectList.append(QRect(layout.senderRect.x(), topAttachment, attachmentsSize.width(), attachmentsSize.height()));
                    topAttachment += attachmentsSize.height();
                } else {
                    const QSize attSize = helper ? helper->sizeHint(msgAttach, index, maxWidth, option) : QSize(0, 0);
                    layout.attachmentsRectList.append(QRect(layout.senderRect.x(), topAttachment, attSize.width(), attSize.height()));
                    attachmentsSize = QSize(qMax(attachmentsSize.width(), attSize.width()), attSize.height() + attachmentsSize.height());
                    topAttachment += attSize.height();
                }
            }
            layout.attachmentsRect = QRect(textLeft, attachmentsY, attachmentsSize.width(), attachmentsSize.height());
        }
        int topBlock = topAttachment;
        if (!message->blocks().isEmpty()) {
            const auto blocks = message->blocks();
            QSize blocksSize;
            for (const Block &block : blocks) {
                const MessageBlockDelegateHelperBase *helper = delegate->blocksHelper(block);
                if (blocksSize.isEmpty()) {
                    blocksSize = helper ? helper->sizeHint(block, index, maxWidth, option) : QSize(0, 0);
                    layout.blocksRectList.append(QRect(layout.senderRect.x(), topBlock, blocksSize.width(), blocksSize.height()));
                    topBlock += blocksSize.height();
                } else {
                    const QSize blockSize = helper ? helper->sizeHint(block, index, maxWidth, option) : QSize(0, 0);
                    layout.blocksRectList.append(QRect(layout.senderRect.x(), topBlock, blockSize.width(), blockSize.height()));
                    blocksSize = QSize(qMax(blocksSize.width(), blockSize.width()), blockSize.height() + blocksSize.height());
                    topBlock += blockSize.height();
                }
            }
            // qDebug() << " topBlock " << topBlock;
            layout.blocksRect = QRect(textLeft, topBlock, blocksSize.width(), blocksSize.height());
        }
        if (mRocketChatAccount && mRocketChatAccount->previewEmbed()) {
            if (!message->urls().isEmpty()) {
                const auto urls = message->urls();
                QSize urlsPreviewSize;
                int topUrlPreview = topBlock;
                for (const MessageUrl &url : urls) {
                    if (url.hasPreviewUrl()) {
                        const MessageDelegateHelperUrlPreview *helperUrlPreview = delegate->helperUrlPreview();
                        if (urlsPreviewSize.isEmpty()) {
                            urlsPreviewSize = helperUrlPreview->sizeHint(url, index, maxWidth, option);
                            layout.messageUrlsRectList.append(QRect(layout.senderRect.x(), topUrlPreview, urlsPreviewSize.width(), urlsPreviewSize.height()));
                            topUrlPreview += urlsPreviewSize.height();
                        } else {
                            const QSize urlPreviewSize = helperUrlPreview->sizeHint(url, index, maxWidth, option);
                            layout.messageUrlsRectList.append(QRect(layout.senderRect.x(), topUrlPreview, urlPreviewSize.width(), urlPreviewSize.height()));
                            urlsPreviewSize = QSize(qMax(urlsPreviewSize.width(), urlPreviewSize.width()), urlPreviewSize.height() + urlsPreviewSize.height());
                            topUrlPreview += urlPreviewSize.height();
                        }
                    } else {
                        layout.messageUrlsRectList.append(QRect{});
                    }
                }
                // qDebug() << " topUrlPreview " << topUrlPreview;
                layout.messageUrlsRect = QRect(textLeft, topUrlPreview, urlsPreviewSize.width(), urlsPreviewSize.height());
            }
        }
        layout.reactionsY = attachmentsY + layout.attachmentsRect.height() + layout.blocksRect.height() + layout.messageUrlsRect.height();
    }
}
