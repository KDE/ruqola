/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistdelegate.h"
using namespace Qt::Literals::StringLiterals;

#include "colorsandmessageviewstyle.h"
#include "common/delegatepaintutil.h"
#include "config-ruqola.h"
#include "delegateutils/messagedelegateutils.h"
#include "delegateutils/textselection.h"
#include "messageattachmentdelegatehelperbase.h"
#include "messageattachmentdelegatehelperfile.h"
#include "messageattachmentdelegatehelperimage.h"
#include "messageattachmentdelegatehelpersound.h"
#include "messageattachmentdelegatehelpertext.h"
#include "messageattachmentdelegatehelpervideo.h"
#include "messagedelegatehelperactions.h"
#include "messagedelegatehelperconferencevideo.h"
#include "messagedelegatehelperreactions.h"
#include "messagedelegatehelpersection.h"
#include "messagedelegatehelpertext.h"
#include "misc/avatarcachemanager.h"
#include "misc/emoticonmenuwidget.h"
#include "model/messagesmodel.h"
#include "rocketchataccount.h"
#include "room/delegate/messagedelegatehelpercontext.h"
#include "room/delegate/messagedelegatehelperdivider.h"
#include "room/delegate/messagelistlayout/messagelistcompactlayout.h"
#include "room/delegate/messagelistlayout/messagelistcozylayout.h"
#include "room/delegate/messagelistlayout/messagelistnormallayout.h"
#include "room/roomutil.h"
#if USE_SIZEHINT_CACHE_SUPPORT
#include "ruqola_sizehint_cache_debug.h"
#endif
#include "messagedelegatehelperurlpreview.h"
#include "ruqola_thread_message_widgets_debug.h"
#include "ruqolaglobalconfig.h"
#include "ruqolawidgets_debug.h"

#include <QAbstractItemView>
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>

#include <KColorScheme>
#include <KLocalizedString>
#include <QMenu>

// #define DEBUG_PAINTING

MessageListDelegate::MessageListDelegate(RocketChatAccount *account, QListView *view)
    : QItemDelegate(view)
    , mEditedIcon(QIcon::fromTheme(u"document-edit"_s))
    , mRolesIcon(QIcon::fromTheme(u"documentinfo"_s))
    , mAddReactionIcon(QIcon::fromTheme(u"smiley-add"_s))
    , mFavoriteIcon(QIcon::fromTheme(u"favorite"_s))
    , mFollowingIcon(QIcon::fromTheme(u"notifications"_s))
    , mPinIcon(QIcon::fromTheme(u"pin"_s))
    , mTranslatedIcon(QIcon::fromTheme(u"translate"_s))
    , mReplyInThreadIcon(QIcon::fromTheme(u"view-conversation-balloon-symbolic"_s))
    , mEncryptedIcon(QIcon::fromTheme(u"document-encrypt"_s))
    , mTextToSpeechIcon(QIcon::fromTheme(u"player-volume"_s))
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
    , mHelperActions(new MessageDelegateHelperActions(account, view, mTextSelectionImpl))
    , mHelperSection(new MessageDelegateHelperSection(account, view, mTextSelectionImpl))
    , mHelperUrlPreview(new MessageDelegateHelperUrlPreview(account, view, mTextSelectionImpl))
    , mHelperDivider(new MessageDelegateHelperDivider(account, view, mTextSelectionImpl))
    , mHelperContext(new MessageDelegateHelperContext(account, view, mTextSelectionImpl))
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::User, this))
    , mMessageListLayoutBase(new MessageListCompactLayout(this))
{
    mTextSelectionImpl->textSelection()->setMessageUrlHelperFactory(mHelperUrlPreview.data());
    mTextSelectionImpl->textSelection()->setTextHelperFactory(mHelperText.data());
    mTextSelectionImpl->textSelection()->setAttachmentFactories({mHelperAttachmentImage.data(),
                                                                 mHelperAttachmentFile.data(),
                                                                 mHelperAttachmentVideo.data(),
                                                                 mHelperAttachmentSound.data(),
                                                                 mHelperAttachmentText.data()});
    // Hardcode color otherwise in dark mode otherwise scheme.background(KColorScheme::NeutralBackground).color(); is not correct for text color.
    mEditColorMode = QColor(255, 170, 127);
    connect(&ColorsAndMessageViewStyle::self(), &ColorsAndMessageViewStyle::needToUpdateColors, this, &MessageListDelegate::slotUpdateColors);
    connect(&ColorsAndMessageViewStyle::self(), &ColorsAndMessageViewStyle::needUpdateMessageStyle, this, &MessageListDelegate::switchMessageLayout);
    connect(&ColorsAndMessageViewStyle::self(), &ColorsAndMessageViewStyle::needUpdateFontSize, this, &MessageListDelegate::clearAvatarSizeHintCache);
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
    const KColorScheme scheme = ColorsAndMessageViewStyle::self().schemeView();
    mThreadedMessageBackgroundColor = ColorsAndMessageViewStyle::self().schemeWindow().background(KColorScheme::AlternateBackground).color();
    mOpenDiscussionColorMode = qApp->palette().link().color();
    mReplyThreadColorMode = scheme.foreground(KColorScheme::NegativeText).color();
    mHoverHightlightColor = scheme.background(KColorScheme::AlternateBackground).color();
    mTranslatedMessageColor = scheme.background(KColorScheme::ActiveBackground).color();
    mTextToSpeechInProgressColor = scheme.background(KColorScheme::PositiveBackground).color();
    Q_EMIT updateView();
}

MessageListLayoutBase::Layout MessageListDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return mMessageListLayoutBase->doLayout(option, index);
}

void MessageListDelegate::setRocketChatAccount(RocketChatAccount *rcAccount)
{
    if (mRocketChatAccount) {
        disconnect(mRocketChatAccount, &RocketChatAccount::ownUserUiPreferencesChanged, this, &MessageListDelegate::switchMessageLayout);
        disconnect(mRocketChatAccount, &RocketChatAccount::privateSettingsChanged, this, &MessageListDelegate::slotPrivateSettingsChanged);
    }
    mRocketChatAccount = rcAccount;
    connect(mRocketChatAccount, &RocketChatAccount::ownUserUiPreferencesChanged, this, &MessageListDelegate::switchMessageLayout);
    connect(mRocketChatAccount, &RocketChatAccount::privateSettingsChanged, this, &MessageListDelegate::slotPrivateSettingsChanged);

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
    mHelperActions->setRocketChatAccount(mRocketChatAccount);
    mHelperSection->setRocketChatAccount(mRocketChatAccount);
    mHelperUrlPreview->setRocketChatAccount(mRocketChatAccount);
    mHelperDivider->setRocketChatAccount(mRocketChatAccount);
    mHelperContext->setRocketChatAccount(mRocketChatAccount);
}

QPixmap MessageListDelegate::makeAvatarPixmap(const QWidget *widget, const QModelIndex &index, int maxHeight) const
{
    const QString emojiStr = index.data(MessagesModel::Emoji).toString();
    const auto info = index.data(MessagesModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (emojiStr.isEmpty()) {
        const QString avatarUrl = index.data(MessagesModel::Avatar).toString();
        if (!avatarUrl.isEmpty()) {
            // qDebug() << " avatarUrl is not empty " << avatarUrl;
            return mAvatarCacheManager->makeAvatarUrlPixmap(widget, avatarUrl, maxHeight);
        } else {
            return mAvatarCacheManager->makeAvatarPixmap(widget, info, maxHeight);
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
    case Block::BlockType::Actions:
        return mHelperActions.get();
    case Block::BlockType::Section:
        return mHelperSection.get();
    case Block::BlockType::Context:
        return mHelperContext.get();
    case Block::BlockType::Divider:
        return mHelperDivider.get();
    case Block::BlockType::Overflow: // TODO look at it!
    case Block::BlockType::Image: // TODO look at it!
    case Block::BlockType::Preview: // TODO look at it!
    case Block::BlockType::Callout: // TODO look at it!
        qCWarning(RUQOLAWIDGETS_LOG) << " not implemented BlockType " << block.blockType();
        return nullptr;
    }
    return nullptr;
}

MessageAttachmentDelegateHelperBase *MessageListDelegate::attachmentsHelper(const MessageAttachment &msgAttach) const
{
    switch (msgAttach.attachmentType()) {
    case MessageAttachment::AttachmentType::Image:
        return mHelperAttachmentImage.data();
    case MessageAttachment::AttachmentType::File:
        return mHelperAttachmentFile.data();
    case MessageAttachment::AttachmentType::Video:
        return mHelperAttachmentVideo.data();
    case MessageAttachment::AttachmentType::Audio:
        return mHelperAttachmentSound.data();
    case MessageAttachment::AttachmentType::NormalText:
        return mHelperAttachmentText.data();
    case MessageAttachment::AttachmentType::Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << "It's an unknown attachment ! It's a bug for sure";
        break;
    }
    return nullptr;
}

void MessageListDelegate::setSearchText(const QString &newSearchText)
{
    if (mHelperText->searchText() != newSearchText) {
        mHelperText->setSearchText(newSearchText);
        if (mPreviewEmbed) {
            mHelperUrlPreview->setSearchText(newSearchText);
        }
        mHelperAttachmentText->setSearchText(newSearchText);
        mHelperAttachmentImage->setSearchText(newSearchText);
        mHelperAttachmentFile->setSearchText(newSearchText);
        mHelperAttachmentVideo->setSearchText(newSearchText);
        mHelperAttachmentSound->setSearchText(newSearchText);
        mSizeHintCache.clear();
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
    const QString dateAndRoomNameStr = u"%1 - %2"_s.arg(index.data(MessagesModel::Date).toString(), roomName);
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

void MessageListDelegate::selectAll([[maybe_unused]] const QStyleOptionViewItem &option, const QModelIndex &index)
{
    mTextSelectionImpl->textSelection()->selectMessage(index);
    mListView->update(index);
    MessageDelegateUtils::setClipboardSelection(mTextSelectionImpl->textSelection());
}

void MessageListDelegate::removeSizeHintCache(const QByteArray &messageId)
{
    mSizeHintCache.remove(messageId);
}

void MessageListDelegate::needUpdateIndexBackground(const QPersistentModelIndex &index, const QColor &color)
{
    removeNeedUpdateIndexBackground(index);
    const IndexBackgroundColor back{.index = index, .color = color};
    mIndexBackgroundColorList.append(std::move(back));
}

void MessageListDelegate::removeNeedUpdateIndexBackground(const QPersistentModelIndex &index)
{
    auto it = std::find_if(mIndexBackgroundColorList.cbegin(), mIndexBackgroundColorList.cend(), [index](const IndexBackgroundColor &key) {
        return key.index == index;
    });
    if (it != mIndexBackgroundColorList.cend()) {
        mIndexBackgroundColorList.erase(it);
    }
}

void MessageListDelegate::removeMessageCache(const Message *message)
{
    const QByteArray messageId = message->messageId();
    removeSizeHintCache(messageId);
    mHelperText->removeMessageCache(messageId);

    if (message->attachments()) {
        const auto attachments{message->attachments()->messageAttachments()};
        for (const auto &attachment : attachments) {
            mHelperAttachmentImage->removeMessageDescriptionTitleCache(attachment);
            mHelperAttachmentFile->removeMessageDescriptionTitleCache(attachment);
            mHelperAttachmentVideo->removeMessageDescriptionTitleCache(attachment);
            mHelperAttachmentSound->removeMessageDescriptionTitleCache(attachment);
            mHelperAttachmentText->removeMessageDescriptionTitleCache(attachment);
        }
    }
    if (mPreviewEmbed) {
        if (message->urls()) {
            const auto messageUrls{message->urls()->messageUrls()};
            for (const auto &url : messageUrls) {
                mHelperUrlPreview->removeMessageCache(url.urlId());
            }
        }
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
    if (mPreviewEmbed) {
        mHelperUrlPreview->clearTextDocumentCache();
    }
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
        if (message->attachments()) {
            const auto attachments = message->attachments()->messageAttachments();
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

        if (mPreviewEmbed) {
            if (message->urls()) {
                const auto urlsMessage = message->urls()->messageUrls();
                int messageUrlIndex = 0;
                for (const MessageUrl &messageUrl : urlsMessage) {
                    url = mHelperUrlPreview->urlAt(option, messageUrl, layout.messageUrlsRectList.at(messageUrlIndex), pos);
                    if (!url.isEmpty()) {
                        return url;
                    }
                    messageUrlIndex++;
                }
            }
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
        auto userInfoAction = new QAction(QIcon::fromTheme(u"documentinfo"_s), i18n("User Info"), &menu);
        connect(userInfoAction, &QAction::triggered, this, [message, this]() {
            Q_EMIT showUserInfo(message->username());
        });

        if (info.editMode) {
            if (info.roomType != Room::RoomType::Direct) {
                if (mRocketChatAccount->hasPermission(u"create-d"_s) && !mRocketChatAccount->offlineMode()) {
                    menu.addSeparator();
                    auto startPrivateConversationAction =
                        new QAction(QIcon::fromTheme(u"document-send-symbolic"_s), i18nc("@action", "Start a Private Conversation"), &menu);
                    connect(startPrivateConversationAction, &QAction::triggered, this, [this, message]() {
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
    } else if (layout.translatedIconRect.contains(info.pos)) {
        QMenu menu;
        const bool isTranslated = message->showTranslatedMessage();
        auto translateAction = new QAction(isTranslated ? i18nc("@action", "Show Original Message") : i18nc("@action", "Translate Message"), &menu);
        connect(translateAction, &QAction::triggered, this, [this, index, isTranslated]() {
            Q_EMIT translateMessage(index, !isTranslated);
        });
        menu.addAction(translateAction);
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
    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
    if (!message) {
        return;
    }
    const MessageListLayoutBase::Layout layout = doLayout(option, index);
    if (message->attachments()) {
        const auto attachments = message->attachments()->messageAttachments();
        int i = 0;
        for (const MessageAttachment &msgAttach : attachments) {
            MessageAttachmentDelegateHelperBase *helper = attachmentsHelper(msgAttach);
            if (helper->contextMenu(info.pos, info.globalPos, msgAttach, layout.attachmentsRectList.at(i), option, menu)) {
                return;
            }
            ++i;
        }
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

    auto it = std::find_if(mIndexBackgroundColorList.cbegin(), mIndexBackgroundColorList.cend(), [index](const IndexBackgroundColor &key) {
        return key.index == index;
    });
    QColor goToMessageBackgroundColor;
    if (it != mIndexBackgroundColorList.cend()) {
        goToMessageBackgroundColor = it->color;
    }
    if (message->textToSpeechInProgress()) {
        painter->fillRect(option.rect, mTextToSpeechInProgressColor);
    } else if (goToMessageBackgroundColor.isValid() && goToMessageBackgroundColor != QColor(Qt::transparent)) {
        painter->fillRect(option.rect, goToMessageBackgroundColor);
    } else if (message->isEditingMode()) {
        painter->fillRect(option.rect, mEditColorMode);
    } else if (message->hoverHighlight() && RuqolaGlobalConfig::self()->showHoverHighlights()) {
        painter->fillRect(option.rect, mHoverHightlightColor);
    } else if (mHelperText->showThreadContext() && !message->threadMessageId().isEmpty()) {
        painter->fillRect(option.rect, mThreadedMessageBackgroundColor);
    } else if (message->showTranslatedMessage() && !message->localTranslation().isEmpty()) {
        painter->fillRect(option.rect, mTranslatedMessageColor);
    } else {
        drawBackground(painter, option, index);
    }

    const MessageListLayoutBase::Layout layout = doLayout(option, index);

    // Draw date if it differs from the previous message
    const bool displayLastSeenMessage = index.data(MessagesModel::DisplayLastSeenMessage).toBool();
    if (message->moderationMessage() && !message->moderationMessage()->isEmpty()) {
        drawModerationDate(painter, index, option, message->moderationMessage()->roomName());
    } else if (index.data(MessagesModel::DateDiffersFromPrevious).toBool()) {
        drawDate(painter, index, option, displayLastSeenMessage);
    } else if (displayLastSeenMessage) {
        drawLastSeenLine(painter, layout.displayLastSeenMessageY, option);
    }

    // Timestamp
    DelegatePaintUtil::drawLighterText(painter, layout.timeStampText, layout.timeStampPos);

    // Message
    if (layout.textRect.isValid()) {
#ifdef DEBUG_PAINTING
        painter->save();
        painter->setPen(QPen(Qt::red));
        painter->drawRect(layout.textRect);
        painter->restore();
#endif
        mHelperText->draw(painter, layout.textRect, index, option);
    }

    // Draw the pixmap
    if (mRocketChatAccount->displayAvatars() && !layout.sameSenderAsPreviousMessage) {
#if USE_ROUNDED_RECT_PIXMAP
        DelegatePaintUtil::createClipRoundedRectangle(painter, QRectF(layout.avatarPos, layout.avatarPixmap.size()), layout.avatarPos, layout.avatarPixmap);
#else
        painter->drawPixmap(layout.avatarPos, layout.avatarPixmap);
#endif
    }

    if (!layout.sameSenderAsPreviousMessage) {
        // Draw the sender
        const QFont oldFont = painter->font();
        painter->setFont(layout.senderFont);
        painter->drawText(layout.senderRect.x(), layout.baseLine, layout.senderText);
        painter->setFont(oldFont);

#ifdef DEBUG_PAINTING
        painter->save();
        painter->setPen(QPen(Qt::red));
        painter->drawRect(layout.senderRect);
        painter->restore();
#endif

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
    // Draw translated string icon
    if (message->isAutoTranslated() || !message->localTranslation().isEmpty()) {
        mTranslatedIcon.paint(painter, layout.translatedIconRect);
    }

    // Draw encrypted icon
    // TODO implement encrypted message
    if (message->isEncryptedMessage()) {
        mEncryptedIcon.paint(painter, layout.encryptedIconRect);
    }

    if (MessageDelegateUtils::showIgnoreMessages(index)) {
        const QIcon hideShowIcon = QIcon::fromTheme(layout.showIgnoreMessage ? u"visibility"_s : u"hint"_s);
        hideShowIcon.paint(painter, layout.showIgnoredMessageIconRect);
    }

    // Attachments
    if (message->attachments()) {
        const auto attachments = message->attachments()->messageAttachments();
        int i = 0;
        for (const MessageAttachment &att : attachments) {
            const MessageAttachmentDelegateHelperBase *helper = attachmentsHelper(att);
            if (helper) {
#ifdef DEBUG_PAINTING
                painter->save();
                painter->setPen(QPen(Qt::green));
                painter->drawRect(layout.attachmentsRectList.at(i));
                painter->restore();
#endif
                helper->draw(att, painter, layout.attachmentsRectList.at(i), index, option);
            }
            ++i;
        }
    }
    // Blocks

    if (message->blocks()) {
        const auto blocks = message->blocks()->blocks();
        int blockIndex = 0;
        for (const Block &block : blocks) {
            const MessageBlockDelegateHelperBase *helper = blocksHelper(block);
            if (helper) {
#ifdef DEBUG_PAINTING
                painter->save();
                painter->setPen(QPen(Qt::red));
                painter->drawRect(layout.blocksRectList.at(blockIndex));
                painter->restore();
#endif
                helper->draw(block, painter, layout.blocksRectList.at(blockIndex), index, option);
            }
            ++blockIndex;
        }
    }

    if (mPreviewEmbed) {
        // Preview Url
        if (message->urls()) {
            const QList<MessageUrl> messageUrls = message->urls()->messageUrls();
            int messageUrlIndex = 0;
            for (const MessageUrl &messageUrl : messageUrls) {
                if (messageUrl.hasPreviewUrl()) {
                    // qDebug() << "messageUrl  " << messageUrl;
                    mHelperUrlPreview.get()->draw(messageUrl, painter, layout.messageUrlsRectList.at(messageUrlIndex), index, option);
                }
                ++messageUrlIndex;
            }
        }
    }

    // Reactions
    const QRect reactionsRect(layout.senderRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
    mHelperReactions->draw(painter, reactionsRect, index, option);

    // Replies
    const int threadCount = message->threadCount();
    if (threadCount > 0) {
        const QString repliesText = i18np("1 reply", "%1 replies", threadCount);
        const QColor oldColor = painter->pen().color();
        painter->setPen(mReplyThreadColorMode);
        painter->drawText(layout.usableRect.x(), layout.repliesY + option.fontMetrics.ascent(), repliesText);
        painter->setPen(oldColor);
    }
    // Discussion
    if (!message->discussionRoomId().isEmpty()) {
        const QColor oldColor = painter->pen().color();
        const int discussionCount{message->discussionCount()};
        const QString discussionsText = (discussionCount > 0) ? i18np("1 message", "%1 messages", discussionCount) : i18n("No message yet");
        painter->setPen(mOpenDiscussionColorMode);
        painter->drawText(layout.usableRect.x(), layout.repliesY + layout.repliesHeight + option.fontMetrics.ascent(), discussionsText);
        // Note: pen still blue, currently relying on restore()
        painter->setPen(oldColor);
    }

    // drawFocus(painter, option, messageRect);

    // debug painter->drawRect(option.rect.adjusted(0, 0, -1, -1));
    if (!isSystemMessage(message) && message->hoverHighlight() && mEmojiMenuEnabled) {
        mAddReactionIcon.paint(painter, layout.addReactionRect, Qt::AlignCenter);
        mReplyInThreadIcon.paint(painter, layout.replyToThreadRect, Qt::AlignCenter);
#if HAVE_TEXT_TO_SPEECH
        if (RuqolaGlobalConfig::self()->enableTextToSpeech()) {
            mTextToSpeechIcon.paint(painter, layout.textToSpeechIconRect, Qt::AlignCenter);
        }
#endif
    }

    painter->restore();
}

void MessageListDelegate::clearSizeHintCache()
{
    mSizeHintCache.clear();
}

QByteArray MessageListDelegate::cacheIdentifier(const QModelIndex &index) const
{
    const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
    Q_ASSERT(message);
    return message->messageId();
}

MessageDelegateHelperUrlPreview *MessageListDelegate::helperUrlPreview() const
{
    return mHelperUrlPreview.get();
}

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
#if USE_SIZEHINT_CACHE_SUPPORT
    const QByteArray identifier = cacheIdentifier(index);
    auto it = mSizeHintCache.find(identifier);
    if (it != mSizeHintCache.end()) {
        const QSize result = it->value;
        qCDebug(RUQOLA_SIZEHINT_CACHE_LOG) << "MessageListDelegate: SizeHint found in cache: " << result;
        return result;
    }
#endif

    const QSize size = mMessageListLayoutBase->sizeHint(option, index);
#if USE_SIZEHINT_CACHE_SUPPORT
    if (!size.isEmpty()) {
        mSizeHintCache.insert(identifier, size);
    }
#endif
    return size;
}

bool MessageListDelegate::isSystemMessage(const Message *message) const
{
    const Message::MessageType messageType = message->messageType();
    const bool isSystemMessage = (messageType == Message::EncryptedText) || (messageType == Message::System)
        || (messageType == Message::Information) /* || (messageType == Message::VideoConference)*/;
    return isSystemMessage || message->privateMessage();
}

bool MessageListDelegate::mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto mev = static_cast<QMouseEvent *>(event);
        const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();

        const MessageListLayoutBase::Layout layout = doLayout(option, index);

        if (!isSystemMessage(message) && mEmojiMenuEnabled) {
            if (layout.addReactionRect.contains(mev->pos())) {
                auto mEmoticonMenuWidget = new EmoticonMenuWidget(mListView);
                mEmoticonMenuWidget->setWindowFlag(Qt::Popup);
                mEmoticonMenuWidget->setCurrentRocketChatAccount(mRocketChatAccount);
                mEmoticonMenuWidget->forceLineEditFocus();
                RoomUtil::positionPopup(mev->globalPosition().toPoint(), mListView, mEmoticonMenuWidget);
                mEmoticonMenuWidget->show();
                connect(mEmoticonMenuWidget, &EmoticonMenuWidget::insertEmojiIdentifier, this, [this, message](const QString &id) {
                    mRocketChatAccount->reactOnMessage(message->messageId(), id, true /*add*/);
                });
                return true;
            } else if (layout.replyToThreadRect.contains(mev->pos())) {
                Q_EMIT replyToThread(message->messageId(), message->text());
                return true;
#if HAVE_TEXT_TO_SPEECH
            } else if (layout.textToSpeechIconRect.contains(mev->pos())) {
                if (message->textToSpeechInProgress()) {
                    Q_EMIT stopTextToSpeech(index);
                } else {
                    Q_EMIT textToSpeech(index);
                }
                return true;
#endif
            }
        }

        if (auto react = message->reactions()) {
            if (!react->isEmpty()) {
                const QRect reactionsRect(layout.senderRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
                if (mHelperReactions->handleMouseEvent(mev, reactionsRect, option, message)) {
                    return true;
                }
            }
        }
        if (message->threadCount() > 0) {
            qCDebug(RUQOLA_THREAD_MESSAGE_WIDGETS_LOG) << "Click on thread area";
            const QRect threadRect(layout.usableRect.x(), layout.repliesY, layout.usableRect.width(), layout.repliesHeight);
            if (threadRect.contains(mev->pos())) {
                const QString threadMessagePreview = index.data(MessagesModel::ThreadMessagePreview).toString();
                qCDebug(RUQOLA_THREAD_MESSAGE_WIDGETS_LOG) << "Click on thread area: " << message->messageId();
                const bool threadIsFollowing = message->replies() ? message->replies()->replies().contains(mRocketChatAccount->userId()) : false;
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
                // qDebug() << " message->discussionRoomId()*****************" << message->discussionRoomId();
                // We need to fix rest api first
                mRocketChatAccount->getRoomByTypeAndName(message->discussionRoomId(), u"c"_s);
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

        if (message->attachments()) {
            const auto attachments = message->attachments()->messageAttachments();
            int i = 0;
            for (const MessageAttachment &att : attachments) {
                MessageAttachmentDelegateHelperBase *helper = attachmentsHelper(att);
                if (helper && helper->handleMouseEvent(att, mev, layout.attachmentsRectList.at(i), option, index)) {
                    return true;
                }
                ++i;
            }
        }

        if (message->blocks()) {
            const auto blocks = message->blocks()->blocks();
            int blockIndex = 0;
            for (const Block &block : blocks) {
                MessageBlockDelegateHelperBase *helper = blocksHelper(block);
                if (helper && helper->handleMouseEvent(block, mev, layout.blocksRectList.at(blockIndex), option, index)) {
                    return true;
                }
                ++blockIndex;
            }
        }
        if (mPreviewEmbed) {
            if (auto urls = message->urls()) {
                const auto messageUrls = urls->messageUrls();
                int messageUrlsIndex = 0;
                for (const MessageUrl &url : messageUrls) {
                    if (mHelperUrlPreview->handleMouseEvent(url, mev, layout.messageUrlsRectList.at(messageUrlsIndex), option, index)) {
                        return true;
                    }
                    ++messageUrlsIndex;
                }
            }
        }
    } else if (eventType == QEvent::MouseButtonPress || eventType == QEvent::MouseMove || eventType == QEvent::MouseButtonDblClick) {
        auto mev = static_cast<QMouseEvent *>(event);
        if (mev->buttons() & Qt::LeftButton) {
            const MessageListLayoutBase::Layout layout = doLayout(option, index);
            if (mHelperText->handleMouseEvent(mev, layout.textRect, option, index)) {
                return true;
            }

            const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
            if (auto messageAttachments = message->attachments()) {
                const auto attachments = messageAttachments->messageAttachments();
                int i = 0;
                for (const MessageAttachment &att : attachments) {
                    MessageAttachmentDelegateHelperBase *helper = attachmentsHelper(att);
                    if (helper && helper->handleMouseEvent(att, mev, layout.attachmentsRectList.at(i), option, index)) {
                        return true;
                    }
                    ++i;
                }
            }
            if (mPreviewEmbed) {
                if (auto urls = message->urls()) {
                    const auto messageUrls = urls->messageUrls();
                    int messageUrlsIndex = 0;
                    for (const MessageUrl &url : messageUrls) {
                        if (mHelperUrlPreview->handleMouseEvent(url, mev, layout.messageUrlsRectList.at(messageUrlsIndex), option, index)) {
                            return true;
                        }
                        ++messageUrlsIndex;
                    }
                }
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
    {
        if (message->attachments()) {
            const auto attachments = message->attachments()->messageAttachments();
            int i = 0;
            for (const MessageAttachment &att : attachments) {
                MessageAttachmentDelegateHelperBase *helper = attachmentsHelper(att);
                if (helper && helper->maybeStartDrag(att, event, layout.attachmentsRectList.at(i), option, index)) {
                    return true;
                }
                ++i;
            }
        }
    }
    {
        if (mPreviewEmbed) {
            if (auto messageUrls = message->urls()) {
                const auto urls = messageUrls->messageUrls();
                int i = 0;
                for (const MessageUrl &url : urls) {
                    if (mHelperUrlPreview->maybeStartDrag(url, event, layout.messageUrlsRectList.at(i), option, index)) {
                        return true;
                    }
                    ++i;
                }
            }
        }
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
        if (auto react = message->reactions()) {
            if (!react->isEmpty()) {
                const QRect reactionsRect(layout.senderRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
                if (mHelperReactions->handleHelpEvent(helpEvent, view, reactionsRect, option, message)) {
                    return true;
                }
            }
        }
        const QPoint helpEventPos{helpEvent->pos()};
        if (layout.senderRect.contains(helpEventPos)) {
            QString tooltip = message->name();

            if (mRocketChatAccount && mRocketChatAccount->ruqolaServerConfig()->useRealName() && !tooltip.isEmpty()) {
                tooltip = u'@' + message->username();
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
            QToolTip::showText(helpEvent->globalPos(), i18nc("@info:tooltip", "Following"), view);
            return true;
        }
        if (layout.pinIconRect.contains(helpEventPos)) {
            QToolTip::showText(helpEvent->globalPos(), i18nc("@info:tooltip", "Message has been pinned"), view);
            return true;
        }
        if (layout.favoriteIconRect.contains(helpEventPos)) {
            QToolTip::showText(helpEvent->globalPos(), i18nc("@info:tooltip", "Message has been starred"), view);
            return true;
        }
        if (layout.translatedIconRect.contains(helpEventPos)) {
            QToolTip::showText(helpEvent->globalPos(), i18nc("@info:tooltip", "Message has a translated text"), view);
            return true;
        }
        if (!isSystemMessage(message) && layout.replyToThreadRect.contains(helpEventPos)) {
            QToolTip::showText(helpEvent->globalPos(), i18nc("@info:tooltip", "Reply in Thread"), view);
            return true;
        }
        if (!isSystemMessage(message) && layout.addReactionRect.contains(helpEventPos)) {
            QToolTip::showText(helpEvent->globalPos(), i18nc("@info:tooltip", "Add reaction"), view);
            return true;
        }
#if HAVE_TEXT_TO_SPEECH
        if (!isSystemMessage(message) && layout.textToSpeechIconRect.contains(helpEventPos) && RuqolaGlobalConfig::self()->enableTextToSpeech()) {
            QToolTip::showText(helpEvent->globalPos(),
                               message->textToSpeechInProgress() ? i18nc("@info:tooltip", "Stop") : i18nc("@info:tooltip", "Speak Text"),
                               view);
            return true;
        }
#endif
        if (layout.textRect.contains(helpEventPos) && mHelperText->handleHelpEvent(helpEvent, layout.textRect, index)) {
            return true;
        }
        // Attachments
        if (message->attachments()) {
            const auto attachments = message->attachments()->messageAttachments();
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
        }

        // Block
        if (message->blocks()) {
            const auto blocks = message->blocks()->blocks();
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
        }

        if (mPreviewEmbed) {
            // messageurls
            if (message->urls()) {
                const auto messageUrls = message->urls()->messageUrls();
                int messageUrlsIndex = 0;
                for (const MessageUrl &url : messageUrls) {
                    if (layout.messageUrlsRectList.at(messageUrlsIndex).contains(helpEventPos)
                        && mHelperUrlPreview->handleHelpEvent(helpEvent, layout.messageUrlsRectList.at(messageUrlsIndex), url, option)) {
                        return true;
                    }
                    ++messageUrlsIndex;
                }
            }
        }
        if (layout.timeStampRect.contains(helpEventPos)) {
            const QString dateStr = index.data(MessagesModel::Date).toString();
            QToolTip::showText(helpEvent->globalPos(), dateStr, view);
            return true;
        }
    }
    return false;
}

void MessageListDelegate::slotPrivateSettingsChanged()
{
    mPreviewEmbed = mRocketChatAccount ? mRocketChatAccount->ruqolaServerConfig()->previewEmbed() : true;
}

void MessageListDelegate::clearAvatarSizeHintCache()
{
    clearSizeHintCache();
    mAvatarCacheManager->clearCache();
}

void MessageListDelegate::switchMessageLayout()
{
    clearAvatarSizeHintCache();
    delete mMessageListLayoutBase;
    switch (RuqolaGlobalConfig::self()->messageStyle()) {
    case RuqolaGlobalConfig::EnumMessageStyle::Normal:
        mMessageListLayoutBase = new MessageListNormalLayout(this);
        break;
    case RuqolaGlobalConfig::EnumMessageStyle::Cozy:
        mMessageListLayoutBase = new MessageListCozyLayout(this);
        break;
    case RuqolaGlobalConfig::EnumMessageStyle::Compact:
        mMessageListLayoutBase = new MessageListCompactLayout(this);
        break;
    default:
        qCWarning(RUQOLAWIDGETS_LOG) << "Invalid Message Layout type " << RuqolaGlobalConfig::self()->messageStyle();
        mMessageListLayoutBase = new MessageListCompactLayout(this);
        break;
    }
    mMessageListLayoutBase->setRocketChatAccount(mRocketChatAccount);
    mPreviewEmbed = mRocketChatAccount ? mRocketChatAccount->ruqolaServerConfig()->previewEmbed() : true;
    Q_EMIT updateView();
}

#include "moc_messagelistdelegate.cpp"
