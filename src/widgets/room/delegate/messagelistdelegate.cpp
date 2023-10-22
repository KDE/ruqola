/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistdelegate.h"
#include "colors.h"
#include "common/delegatepaintutil.h"
#include "delegateutils/messagedelegateutils.h"
#include "delegateutils/textselection.h"
#include "messageattachmentdelegatehelperbase.h"
#include "messageattachmentdelegatehelperfile.h"
#include "messageattachmentdelegatehelperimage.h"
#include "messageattachmentdelegatehelpersound.h"
#include "messageattachmentdelegatehelpertext.h"
#include "messageattachmentdelegatehelpervideo.h"
#include "messagedelegatehelperconferencevideo.h"
#include "messagedelegatehelperreactions.h"
#include "messagedelegatehelpertext.h"
#include "misc/avatarcachemanager.h"
#include "misc/emoticonmenuwidget.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "room/delegate/messagelistlayout/messagelistcompactlayout.h"
#include "room/delegate/messagelistlayout/messagelistcozylayout.h"
#include "room/delegate/messagelistlayout/messagelistnormallayout.h"
#include "ruqola_thread_message_widgets_debug.h"
#include "ruqolaglobalconfig.h"
#include "ruqolawidgets_debug.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QClipboard>
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QToolTip>

#include <KColorScheme>
#include <KLocalizedString>
#include <QMenu>

MessageListDelegate::MessageListDelegate(RocketChatAccount *account, QListView *view)
    : QItemDelegate(view)
    , mEditedIcon(QIcon::fromTheme(QStringLiteral("document-edit")))
    , mRolesIcon(QIcon::fromTheme(QStringLiteral("documentinfo")))
    , mAddReactionIcon(QIcon::fromTheme(QStringLiteral("smiley-add")))
    , mFavoriteIcon(QIcon::fromTheme(QStringLiteral("favorite")))
    , mFollowingIcon(QIcon::fromTheme(QStringLiteral("notifications")))
    , mPinIcon(QIcon::fromTheme(QStringLiteral("pin")))
    , mTranslatedIcon(QIcon::fromTheme(QStringLiteral("languages"))) // TODO use another icon for it. But kde doesn't correct icon perhaps flags ?
    , mListView(view)
    , mTextSelectionImpl(new TextSelectionImpl)
    , mHelperText(new MessageDelegateHelperText(account, view, mTextSelectionImpl))
    , mHelperAttachmentImage(new MessageAttachmentDelegateHelperImage(account, view, mTextSelectionImpl))
    , mHelperAttachmentFile(new MessageAttachmentDelegateHelperFile(account, view, mTextSelectionImpl))
    , mHelperReactions(new MessageDelegateHelperReactions(account))
    , mHelperAttachmentVideo(new MessageAttachmentDelegateHelperVideo(account, view, mTextSelectionImpl))
    , mHelperAttachmentSound(new MessageAttachmentDelegateHelperSound(account, view, mTextSelectionImpl))
    , mHelperAttachmentText(new MessageAttachmentDelegateHelperText(account, view, mTextSelectionImpl))
    , mHelperConferenceVideo(new MessageDelegateHelperConferenceVideo(account, view, mTextSelectionImpl))
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::User, this))
    , mMessageListLayoutBase(new MessageListCompactLayout(this))
{
    mTextSelectionImpl->textSelection()->setTextHelperFactory(mHelperText.data());
    mTextSelectionImpl->textSelection()->setAttachmentFactories({mHelperAttachmentImage.data(),
                                                                 mHelperAttachmentFile.data(),
                                                                 mHelperAttachmentVideo.data(),
                                                                 mHelperAttachmentSound.data(),
                                                                 mHelperAttachmentText.data()});
    // Hardcode color otherwise in dark mode otherwise scheme.background(KColorScheme::NeutralBackground).color(); is not correct for text color.
    mEditColorMode = QColor(255, 170, 127);
    connect(&Colors::self(), &Colors::needToUpdateColors, this, &MessageListDelegate::slotUpdateColors);
    slotUpdateColors();
    mSizeHintCache.setMaxEntries(32); // Enough ?
}

MessageListDelegate::~MessageListDelegate()
{
    delete mTextSelectionImpl;
    delete mMessageListLayoutBase;
}

void MessageListDelegate::slotUpdateColors()
{
    const KColorScheme scheme = Colors::self().schemeView();
    mThreadedMessageBackgroundColor = Colors::self().schemeWindow().background(KColorScheme::AlternateBackground).color();
    mOpenDiscussionColorMode = scheme.foreground(KColorScheme::LinkText).color();
    mReplyThreadColorMode = scheme.foreground(KColorScheme::NegativeText).color();
    mHoverHightlightColor = scheme.background(KColorScheme::AlternateBackground).color();
    Q_EMIT updateView();
}

MessageListLayoutBase::Layout MessageListDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return mMessageListLayoutBase->doLayout(option, index);
}

void MessageListDelegate::setRocketChatAccount(RocketChatAccount *rcAccount)
{
    if (mRocketChatAccount) {
        disconnect(mRocketChatAccount, &RocketChatAccount::ownUserPreferencesChanged, this, &MessageListDelegate::switchMessageLayout);
    }
    mRocketChatAccount = rcAccount;
    connect(mRocketChatAccount, &RocketChatAccount::ownUserPreferencesChanged, this, &MessageListDelegate::switchMessageLayout);

    mAvatarCacheManager->setCurrentRocketChatAccount(mRocketChatAccount);
    // Switch messageLayout after set rocketchatAccount
    switchMessageLayout();

    mHelperText->setRocketChatAccount(mRocketChatAccount);
    mHelperAttachmentImage->setRocketChatAccount(mRocketChatAccount);
    mHelperAttachmentFile->setRocketChatAccount(mRocketChatAccount);
    mHelperReactions->setRocketChatAccount(mRocketChatAccount);
    mHelperAttachmentVideo->setRocketChatAccount(mRocketChatAccount);
    mHelperAttachmentSound->setRocketChatAccount(mRocketChatAccount);
    mHelperAttachmentText->setRocketChatAccount(mRocketChatAccount);
    mHelperConferenceVideo->setRocketChatAccount(mRocketChatAccount);
}

QPixmap MessageListDelegate::makeAvatarPixmap(const QWidget *widget, const QModelIndex &index, int maxHeight) const
{
    const QString emojiStr = index.data(MessagesModel::Emoji).toString();
    const auto info = index.data(MessagesModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (emojiStr.isEmpty()) {
        const QString avatarUrl = index.data(MessagesModel::Avatar).toString();
        if (!avatarUrl.isEmpty()) {
            // TODO
            // qDebug() << " avatarUrl is not empty " << avatarUrl;
            return mAvatarCacheManager->makeAvatarUrlPixmap(widget, info, maxHeight);
        } else {
            return mAvatarCacheManager->makeAvatarUrlPixmap(widget, info, maxHeight);
        }
    } else {
        return mAvatarCacheManager->makeAvatarEmojiPixmap(emojiStr, widget, info, maxHeight);
    }
}

MessageBlockDelegateHelperBase *MessageListDelegate::blocksHelper(const Block &block) const
{
    switch (block.blockType()) {
    case Block::BlockType::Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << "It's an unknown block ! It's a bug for sure";
        return nullptr;
    case Block::BlockType::VideoConf:
        return mHelperConferenceVideo.get();
    }
    return nullptr;
}

MessageAttachmentDelegateHelperBase *MessageListDelegate::attachmentsHelper(const MessageAttachment &msgAttach) const
{
    switch (msgAttach.attachmentType()) {
    case MessageAttachment::Image:
        return mHelperAttachmentImage.data();
    case MessageAttachment::File:
        return mHelperAttachmentFile.data();
    case MessageAttachment::Video:
        return mHelperAttachmentVideo.data();
    case MessageAttachment::Audio:
        return mHelperAttachmentSound.data();
    case MessageAttachment::NormalText:
        return mHelperAttachmentText.data();
    case MessageAttachment::Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << "It's an unknown attachment ! It's a bug for sure";
        break;
    }
    return nullptr;
}

void MessageListDelegate::setSearchText(const QString &newSearchText)
{
    if (mHelperText->searchText() != newSearchText) {
        mHelperText->setSearchText(newSearchText);
        clearTextDocumentCache();
    }
}

void MessageListDelegate::drawLastSeenLine(QPainter *painter, qint64 displayLastSeenY, const QStyleOptionViewItem &option) const
{
    const QPen origPen = painter->pen();
    const int lineY = displayLastSeenY;
    painter->setPen(Qt::red);
    painter->drawLine(option.rect.x(), lineY, option.rect.width(), lineY);
    painter->setPen(origPen);
}

MessageDelegateHelperReactions *MessageListDelegate::helperReactions() const
{
    return mHelperReactions.data();
}

MessageDelegateHelperText *MessageListDelegate::helperText() const
{
    return mHelperText.data();
}

void MessageListDelegate::drawModerationDate(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option, const QString &roomName) const
{
    const QPen origPen = painter->pen();
    const qreal margin = MessageDelegateUtils::basicMargin();
    const QString dateAndRoomNameStr = QStringLiteral("%1 - %2").arg(index.data(MessagesModel::Date).toString(), roomName);
    const QSize dateSize = option.fontMetrics.size(Qt::TextSingleLine, dateAndRoomNameStr);
    const QRect dateAreaRect(option.rect.x(), option.rect.y(), option.rect.width(), dateSize.height()); // the whole row
    const QRect dateTextRect = QStyle::alignedRect(Qt::LayoutDirectionAuto, Qt::AlignCenter, dateSize, dateAreaRect);
    painter->drawText(dateTextRect, dateAndRoomNameStr);
    const int lineY = (dateAreaRect.top() + dateAreaRect.bottom()) / 2;
    QColor lightColor(painter->pen().color());
    lightColor.setAlpha(60);
    painter->setPen(lightColor);
    painter->drawLine(dateAreaRect.left(), lineY, dateTextRect.left() - margin, lineY);
    painter->drawLine(dateTextRect.right() + margin, lineY, dateAreaRect.right(), lineY);
    painter->setPen(origPen);
}

void MessageListDelegate::drawDate(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option, bool drawLastSeenLine) const
{
    const QPen origPen = painter->pen();
    const qreal margin = MessageDelegateUtils::basicMargin();
    const QString dateStr = index.data(MessagesModel::Date).toString();
    const QSize dateSize = option.fontMetrics.size(Qt::TextSingleLine, dateStr);
    const QRect dateAreaRect(option.rect.x(), option.rect.y(), option.rect.width(), dateSize.height()); // the whole row
    const QRect dateTextRect = QStyle::alignedRect(Qt::LayoutDirectionAuto, Qt::AlignCenter, dateSize, dateAreaRect);
    painter->drawText(dateTextRect, dateStr);
    const int lineY = (dateAreaRect.top() + dateAreaRect.bottom()) / 2;
    if (drawLastSeenLine) {
        painter->setPen(Qt::red);
    } else {
        QColor lightColor(painter->pen().color());
        lightColor.setAlpha(60);
        painter->setPen(lightColor);
    }
    painter->drawLine(dateAreaRect.left(), lineY, dateTextRect.left() - margin, lineY);
    painter->drawLine(dateTextRect.right() + margin, lineY, dateAreaRect.right(), lineY);
    painter->setPen(origPen);
}

void MessageListDelegate::selectAll(const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(option);
    mTextSelectionImpl->textSelection()->selectMessage(index);
    mListView->update(index);
    MessageDelegateUtils::setClipboardSelection(mTextSelectionImpl->textSelection());
}

void MessageListDelegate::removeMessageCache(const QString &messageId, const QStringList &attachmentIdList)
{
    mSizeHintCache.remove(messageId);
    mHelperText->removeMessageCache(messageId);
    for (const auto &attachmentId : attachmentIdList) {
        mHelperAttachmentImage->removeMessageCache(attachmentId);
        mHelperAttachmentFile->removeMessageCache(attachmentId);
        mHelperAttachmentVideo->removeMessageCache(attachmentId);
        mHelperAttachmentSound->removeMessageCache(attachmentId);
        mHelperAttachmentText->removeMessageCache(attachmentId);
    }
}

void MessageListDelegate::clearTextDocumentCache()
{
    mSizeHintCache.clear();
    mHelperText->clearTextDocumentCache();
    mHelperAttachmentImage->clearTextDocumentCache();
    mHelperAttachmentFile->clearTextDocumentCache();
    mHelperAttachmentVideo->clearTextDocumentCache();
    mHelperAttachmentSound->clearTextDocumentCache();
    mHelperAttachmentText->clearTextDocumentCache();
}

void MessageListDelegate::clearSelection()
{
    mTextSelectionImpl->textSelection()->clear();
}

QString MessageListDelegate::urlAt(const QStyleOptionViewItem &option, const QModelIndex &index, QPoint pos) const
{
    const MessageListLayoutBase::Layout layout = doLayout(option, index);
    const auto messageRect = layout.textRect;
    QString url = mHelperText->urlAt(index, pos - messageRect.topLeft());
    if (url.isEmpty()) {
        const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
        Q_ASSERT(message);
        const auto attachments = message->attachments();
        int i = 0;
        for (const MessageAttachment &msgAttach : attachments) {
            MessageAttachmentDelegateHelperBase *helper = attachmentsHelper(msgAttach);
            url = helper->urlAt(option, msgAttach, layout.attachmentsRectList.at(i), pos);
            if (!url.isEmpty()) {
                return url;
            }
            i++;
        }
    }
    return url;
}

bool MessageListDelegate::contextMenu(const QStyleOptionViewItem &option, const QModelIndex &index, const MessageListDelegate::MenuInfo &info)
{
    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
    if (!message) {
        return false;
    }

    const MessageListLayoutBase::Layout layout = doLayout(option, index);
    if (layout.senderRect.contains(info.pos) && !layout.sameSenderAsPreviousMessage) {
        QMenu menu;
        auto userInfoAction = new QAction(QIcon::fromTheme(QStringLiteral("documentinfo")), i18n("User Info"), &menu);
        connect(userInfoAction, &QAction::triggered, this, [message, this]() {
            Q_EMIT showUserInfo(message->username());
        });

        if (info.editMode) {
            if (info.roomType != Room::RoomType::Direct) {
                if (mRocketChatAccount->hasPermission(QStringLiteral("create-d"))) {
                    menu.addSeparator();
                    auto startPrivateConversationAction = new QAction(i18n("Start a Private Conversation"), &menu);
                    connect(startPrivateConversationAction, &QAction::triggered, this, [=]() {
                        Q_EMIT startPrivateConversation(message->username());
                    });
                    menu.addAction(startPrivateConversationAction);
                }
            }
        }

        menu.addSeparator();
        menu.addAction(userInfoAction);
        menu.exec(info.globalPos);
        return true;
    }
    return false;
}

void MessageListDelegate::attachmentContextMenu(const QStyleOptionViewItem &option,
                                                const QModelIndex &index,
                                                const MessageListDelegate::MenuInfo &info,
                                                QMenu *menu)
{
    const MessageListLayoutBase::Layout layout = doLayout(option, index);
    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
    if (!message) {
        return;
    }
    const auto attachments = message->attachments();
    int i = 0;
    for (const MessageAttachment &msgAttach : attachments) {
        MessageAttachmentDelegateHelperBase *helper = attachmentsHelper(msgAttach);
        if (helper->contextMenu(info.pos, info.globalPos, msgAttach, layout.attachmentsRectList.at(i), option, menu)) {
            return;
        }
        ++i;
    }
}

QString MessageListDelegate::selectedText() const
{
    return mTextSelectionImpl->textSelection()->selectedText(TextSelection::Format::Text);
}

bool MessageListDelegate::hasSelection() const
{
    return mTextSelectionImpl->textSelection()->hasSelection();
}

void MessageListDelegate::setShowThreadContext(bool b)
{
    mHelperText->setShowThreadContext(b);
}

void MessageListDelegate::setEnableEmojiMenu(bool b)
{
    mEmojiMenuEnabled = b;
}

void MessageListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();

    const QColor goToMessageBackgroundColor{message->goToMessageBackgroundColor()};
    if (goToMessageBackgroundColor.isValid() && goToMessageBackgroundColor != QColor(Qt::transparent)) {
        painter->fillRect(option.rect, goToMessageBackgroundColor);
    } else if (message->isEditingMode()) {
        painter->fillRect(option.rect, mEditColorMode);
    } else if (message->hoverHighlight() && RuqolaGlobalConfig::self()->showHoverHighlights()) {
        painter->fillRect(option.rect, mHoverHightlightColor);
    } else if (mHelperText->showThreadContext() && !message->threadMessageId().isEmpty()) {
        painter->fillRect(option.rect, mThreadedMessageBackgroundColor);
    } else {
        drawBackground(painter, option, index);
    }

    const MessageListLayoutBase::Layout layout = doLayout(option, index);

    // Draw date if it differs from the previous message
    const bool displayLastSeenMessage = index.data(MessagesModel::DisplayLastSeenMessage).toBool();
    if (!message->moderationMessage().isEmpty()) {
        drawModerationDate(painter, index, option, message->moderationMessage().roomName());
    } else if (index.data(MessagesModel::DateDiffersFromPrevious).toBool()) {
        drawDate(painter, index, option, displayLastSeenMessage);
    } else if (displayLastSeenMessage) {
        drawLastSeenLine(painter, layout.displayLastSeenMessageY, option);
    }

    // Timestamp
    DelegatePaintUtil::drawLighterText(painter, layout.timeStampText, layout.timeStampPos);
    if (!isSystemMessage(message) && message->hoverHighlight() && mEmojiMenuEnabled) {
        mAddReactionIcon.paint(painter, layout.addReactionRect, Qt::AlignCenter);
    }

    // Message
    if (layout.textRect.isValid()) {
#if 0
        painter->save();
        painter->setPen(QPen(Qt::red));
        painter->drawRect(layout.textRect);
        painter->restore();
#endif
        mHelperText->draw(painter, layout.textRect, index, option);
    }

    // Draw the pixmap
    if (mRocketChatAccount->displayAvatars() && !layout.sameSenderAsPreviousMessage) {
        painter->drawPixmap(layout.avatarPos, layout.avatarPixmap);
    }

    if (!layout.sameSenderAsPreviousMessage) {
        // Draw the sender
        const QFont oldFont = painter->font();
        painter->setFont(layout.senderFont);
        painter->drawText(layout.senderRect.x(), layout.baseLine, layout.senderText);
        painter->setFont(oldFont);

        // Draw the roles icon
        if (!index.data(MessagesModel::Roles).toString().isEmpty() && !mRocketChatAccount->hideRoles()) {
            mRolesIcon.paint(painter, layout.rolesIconRect);
        }
    }

    // Draw the edited icon
    if (message->wasEdited()) {
        mEditedIcon.paint(painter, layout.editedIconRect);
    }
    // Draw the favorite icon
    if (message->isStarred()) {
        mFavoriteIcon.paint(painter, layout.favoriteIconRect);
    }
    // Draw the pin icon
    if (message->isPinned()) {
        mPinIcon.paint(painter, layout.pinIconRect);
    }
    // Draw the following icon
    if (layout.messageIsFollowing) {
        mFollowingIcon.paint(painter, layout.followingIconRect);
    }
    // Draw translated string
    if (message->isAutoTranslated()) {
        mTranslatedIcon.paint(painter, layout.translatedIconRect);
    }

    if (MessageDelegateUtils::showIgnoreMessages(index)) {
        const QIcon hideShowIcon = QIcon::fromTheme(layout.showIgnoreMessage ? QStringLiteral("visibility") : QStringLiteral("hint"));
        hideShowIcon.paint(painter, layout.showIgnoredMessageIconRect);
    }

    // Attachments
    const auto attachments = message->attachments();
    int i = 0;
    for (const MessageAttachment &att : attachments) {
        const MessageAttachmentDelegateHelperBase *helper = attachmentsHelper(att);
        if (helper) {
#if 0
            painter->save();
            painter->setPen(QPen(Qt::green));
            painter->drawRect(layout.attachmentsRectList.at(i));
            painter->restore();
#endif
            helper->draw(att, painter, layout.attachmentsRectList.at(i), index, option);
        }
        ++i;
    }
    // Blocks

    const auto blocks = message->blocks();
    int blockIndex = 0;
    for (const Block &block : blocks) {
        const MessageBlockDelegateHelperBase *helper = blocksHelper(block);
        if (helper) {
#if 0
            painter->save();
            painter->setPen(QPen(Qt::red));
            painter->drawRect(layout.blocksRectList.at(blockIndex));
            painter->restore();
#endif
            helper->draw(block, painter, layout.blocksRectList.at(blockIndex), index, option);
        }
        ++blockIndex;
    }

    // Reactions
    const QRect reactionsRect(layout.senderRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
    mHelperReactions->draw(painter, reactionsRect, index, option);

    // Replies
    const int threadCount = message->threadCount();
    if (threadCount > 0) {
        const QString repliesText = i18np("1 reply", "%1 replies", threadCount);
        painter->setPen(mReplyThreadColorMode);
        painter->drawText(layout.usableRect.x(), layout.repliesY + option.fontMetrics.ascent(), repliesText);
    }
    // Discussion
    if (!message->discussionRoomId().isEmpty()) {
        const QString discussionsText =
            (message->discussionCount() > 0) ? i18np("1 message", "%1 messages", message->discussionCount()) : i18n("No message yet");
        painter->setPen(mOpenDiscussionColorMode);
        painter->drawText(layout.usableRect.x(), layout.repliesY + layout.repliesHeight + option.fontMetrics.ascent(), discussionsText);
        // Note: pen still blue, currently relying on restore()
    }

    // drawFocus(painter, option, messageRect);

    // debug painter->drawRect(option.rect.adjusted(0, 0, -1, -1));

    painter->restore();
}

void MessageListDelegate::clearSizeHintCache()
{
    mSizeHintCache.clear();
}

QString MessageListDelegate::cacheIdentifier(const QModelIndex &index) const
{
    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
    Q_ASSERT(message);
    return message->messageId();
}

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QString identifier = cacheIdentifier(index);
    auto it = mSizeHintCache.find(identifier);
    if (it != mSizeHintCache.end()) {
        qDebug() << " found !!!!";
        return it->value;
    }

    const QSize size = mMessageListLayoutBase->sizeHint(option, index);
    mSizeHintCache.insert(identifier, size);
    return size;
}

static void positionPopup(QPoint pos, QWidget *parentWindow, QWidget *popup)
{
    const QRect screenRect = parentWindow->screen()->availableGeometry();

    const QSize popupSize{popup->sizeHint()};
    QRect popupRect(QPoint(pos.x() - popupSize.width(), pos.y() - popupSize.height()), popup->sizeHint());
    if (popupRect.top() < screenRect.top()) {
        popupRect.moveTop(screenRect.top());
    }

    if ((pos.x() + popupSize.width()) > (screenRect.x() + screenRect.width())) {
        popupRect.setX(screenRect.x() + screenRect.width() - popupSize.width());
    }
    if (pos.x() - popupSize.width() < screenRect.x()) {
        popupRect.setX(screenRect.x());
    }

    popup->setGeometry(popupRect);
}

bool MessageListDelegate::isSystemMessage(const Message *message) const
{
    const Message::MessageType messageType = message->messageType();
    const bool isSystemMessage = (messageType == Message::System) || (messageType == Message::Information) || (messageType == Message::VideoConference);
    return isSystemMessage;
}

bool MessageListDelegate::mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto mev = static_cast<QMouseEvent *>(event);
        const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();

        const MessageListLayoutBase::Layout layout = doLayout(option, index);

        if (layout.addReactionRect.contains(mev->pos()) && !isSystemMessage(message)) {
            auto mEmoticonMenuWidget = new EmoticonMenuWidget(mListView);
            mEmoticonMenuWidget->setWindowFlag(Qt::Popup);
            mEmoticonMenuWidget->setCurrentRocketChatAccount(mRocketChatAccount);
            mEmoticonMenuWidget->forceLineEditFocus();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            positionPopup(mev->globalPos(), mListView, mEmoticonMenuWidget);
#else
            positionPopup(mev->globalPosition().toPoint(), mListView, mEmoticonMenuWidget);
#endif
            mEmoticonMenuWidget->show();
            connect(mEmoticonMenuWidget, &EmoticonMenuWidget::insertEmojiIdentifier, this, [=](const QString &id) {
                mRocketChatAccount->reactOnMessage(message->messageId(), id, true /*add*/);
            });
            return true;
        }

        if (!message->reactions().isEmpty()) {
            const QRect reactionsRect(layout.senderRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
            if (mHelperReactions->handleMouseEvent(mev, reactionsRect, option, message)) {
                return true;
            }
        }
        if (message->threadCount() > 0) {
            qCDebug(RUQOLA_THREAD_MESSAGE_WIDGETS_LOG) << "Click on thread area";
            const QRect threadRect(layout.usableRect.x(), layout.repliesY, layout.usableRect.width(), layout.repliesHeight);
            if (threadRect.contains(mev->pos())) {
                const QString threadMessagePreview = index.data(MessagesModel::ThreadMessagePreview).toString();
                qCDebug(RUQOLA_THREAD_MESSAGE_WIDGETS_LOG) << "Click on thread area: " << message->messageId();
                const bool threadIsFollowing = message->replies().contains(mRocketChatAccount->userId());
                // We show current => use this message
                const Message threadMessage = *message;
                Q_EMIT mRocketChatAccount->openThreadRequested(message->messageId(),
                                                               threadMessagePreview.isEmpty() ? index.data(MessagesModel::MessageConvertedText).toString()
                                                                                              : threadMessagePreview,
                                                               threadIsFollowing,
                                                               threadMessage);
                return true;
            }
        }
        if (!message->discussionRoomId().isEmpty()) {
            const QRect discussionRect(layout.usableRect.x(), layout.repliesY + layout.repliesHeight, layout.usableRect.width(), layout.discussionsHeight);
            if (discussionRect.contains(mev->pos())) {
                // We need to fix rest api first
                mRocketChatAccount->joinDiscussion(message->discussionRoomId(), QString());
                return true;
            }
        }
        if (MessageDelegateUtils::showIgnoreMessages(index)) {
            if (layout.showIgnoredMessageIconRect.contains(mev->pos())) {
                mHelperText->removeMessageCache(message->messageId());
                auto model = const_cast<QAbstractItemModel *>(index.model());
                model->setData(index, !layout.showIgnoreMessage, MessagesModel::ShowIgnoredMessage);
                return true;
            }
        }
        if (mHelperText->handleMouseEvent(mev, layout.textRect, option, index)) {
            return true;
        }

        const auto attachments = message->attachments();
        int i = 0;
        for (const MessageAttachment &att : attachments) {
            MessageAttachmentDelegateHelperBase *helper = attachmentsHelper(att);
            if (helper && helper->handleMouseEvent(att, mev, layout.attachmentsRectList.at(i), option, index)) {
                return true;
            }
            ++i;
        }

        const auto blocks = message->blocks();
        int blockIndex = 0;
        for (const Block &block : blocks) {
            MessageBlockDelegateHelperBase *helper = blocksHelper(block);
            if (helper && helper->handleMouseEvent(block, mev, layout.blocksRectList.at(blockIndex), option, index)) {
                return true;
            }
            ++blockIndex;
        }
    } else if (eventType == QEvent::MouseButtonPress || eventType == QEvent::MouseMove || eventType == QEvent::MouseButtonDblClick) {
        auto mev = static_cast<QMouseEvent *>(event);
        if (mev->buttons() & Qt::LeftButton) {
            const MessageListLayoutBase::Layout layout = doLayout(option, index);
            if (mHelperText->handleMouseEvent(mev, layout.textRect, option, index)) {
                return true;
            }

            const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
            const auto attachments = message->attachments();
            int i = 0;
            for (const MessageAttachment &att : attachments) {
                MessageAttachmentDelegateHelperBase *helper = attachmentsHelper(att);
                if (helper && helper->handleMouseEvent(att, mev, layout.attachmentsRectList.at(i), option, index)) {
                    return true;
                }
                ++i;
            }
        }
    }
    return false;
}

bool MessageListDelegate::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const MessageListLayoutBase::Layout layout = doLayout(option, index);
    if (mHelperText->maybeStartDrag(event, layout.textRect, option, index)) {
        return true;
    }

    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
    const auto attachments = message->attachments();
    int i = 0;
    for (const MessageAttachment &att : attachments) {
        MessageAttachmentDelegateHelperBase *helper = attachmentsHelper(att);
        if (helper && helper->maybeStartDrag(att, event, layout.attachmentsRectList.at(i), option, index)) {
            return true;
        }
        ++i;
    }
    const auto blocks = message->blocks();
    int blockIndex = 0;
    for (const Block &block : blocks) {
        MessageBlockDelegateHelperBase *helper = blocksHelper(block);
        if (helper) {
            if (helper && helper->maybeStartDrag(block, event, layout.blocksRectList.at(blockIndex), option, index)) {
                return true;
            }
        }
        ++blockIndex;
    }
    return false;
}

bool MessageListDelegate::helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (helpEvent->type() == QEvent::ToolTip) {
        const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
        if (!message) {
            // tooltip was requested in an empty space below the last message, nothing to do
            return false;
        }

        const MessageListLayoutBase::Layout layout = doLayout(option, index);
        if (!message->reactions().isEmpty()) {
            const QRect reactionsRect(layout.senderRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
            if (mHelperReactions->handleHelpEvent(helpEvent, view, reactionsRect, option, message)) {
                return true;
            }
        }
        const QPoint helpEventPos{helpEvent->pos()};
        if (layout.senderRect.contains(helpEventPos)) {
            QString tooltip = message->name();

            if (mRocketChatAccount && mRocketChatAccount->useRealName() && !tooltip.isEmpty()) {
                tooltip = QLatin1Char('@') + message->username();
            }

            if (!tooltip.isEmpty()) {
                QToolTip::showText(helpEvent->globalPos(), tooltip, view);
                return true;
            }
        }
        if (layout.rolesIconRect.contains(helpEventPos)) {
            const QString tooltip = index.data(MessagesModel::Roles).toString();
            QToolTip::showText(helpEvent->globalPos(), tooltip, view);
            return true;
        }
        if (layout.editedIconRect.contains(helpEventPos)) {
            const QString tooltip = index.data(MessagesModel::EditedToolTip).toString();
            QToolTip::showText(helpEvent->globalPos(), tooltip, view);
            return true;
        }
        if (layout.followingIconRect.contains(helpEventPos)) {
            QToolTip::showText(helpEvent->globalPos(), i18n("Following"), view);
            return true;
        }
        if (layout.pinIconRect.contains(helpEventPos)) {
            QToolTip::showText(helpEvent->globalPos(), i18n("Message has been pinned"), view);
            return true;
        }
        if (layout.favoriteIconRect.contains(helpEventPos)) {
            QToolTip::showText(helpEvent->globalPos(), i18n("Message has been starred"), view);
            return true;
        }
        if (layout.textRect.contains(helpEvent->pos()) && mHelperText->handleHelpEvent(helpEvent, layout.textRect, index)) {
            return true;
        }
        // Attachments
        const auto attachments = message->attachments();
        int i = 0;
        for (const MessageAttachment &att : attachments) {
            MessageAttachmentDelegateHelperBase *helper = attachmentsHelper(att);
            if (helper) {
                if (layout.attachmentsRectList.at(i).contains(helpEventPos)
                    && helper->handleHelpEvent(helpEvent, layout.attachmentsRectList.at(i), att, option)) {
                    return true;
                }
            }
            ++i;
        }

        const auto blocks = message->blocks();
        int blockIndex = 0;
        for (const Block &block : blocks) {
            MessageBlockDelegateHelperBase *helper = blocksHelper(block);
            if (helper) {
                if (layout.blocksRectList.at(blockIndex).contains(helpEventPos)
                    && helper->handleHelpEvent(helpEvent, layout.blocksRectList.at(blockIndex), block, option)) {
                    return true;
                }
            }
            ++blockIndex;
        }

        if (layout.timeStampRect.contains(helpEvent->pos())) {
            const QString dateStr = index.data(MessagesModel::Date).toString();
            QToolTip::showText(helpEvent->globalPos(), dateStr, view);
            return true;
        }
    }
    return false;
}

void MessageListDelegate::switchMessageLayout()
{
    // FIXME reactivate it when layout will be implemented correctly
    // Move this line at end when we reactivate code
#if 1
    mMessageListLayoutBase->setRocketChatAccount(mRocketChatAccount);
#else
    if (mRocketChatAccount) {
        delete mMessageListLayoutBase;
        const int i = mRocketChatAccount->ownUserPreferences().messageViewMode();
        switch (i) {
        case 0:
            mMessageListLayoutBase = new MessageListNormalLayout(this);
            break;
        case 1:
            mMessageListLayoutBase = new MessageListCozyLayout(this);
            break;
        case 2:
            mMessageListLayoutBase = new MessageListCompactLayout(this);
            break;
        default:
            qCWarning(RUQOLAWIDGETS_LOG) << "Invalid Message Layout type " << i;
            mMessageListLayoutBase = new MessageListCompactLayout(this);
            break;
        }
        mMessageListLayoutBase->setRocketChatAccount(mRocketChatAccount);
        updateView();
    }
#endif
}

#include "moc_messagelistdelegate.cpp"
