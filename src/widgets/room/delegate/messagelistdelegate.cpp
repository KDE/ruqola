/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagelistdelegate.h"
#include "colors.h"
#include "common/delegatepaintutil.h"
#include "delegateutils/messagedelegateutils.h"
#include "delegateutils/textselection.h"
#include "emoticons/emojimanager.h"
#include "messageattachmentdelegatehelperfile.h"
#include "messageattachmentdelegatehelperimage.h"
#include "messageattachmentdelegatehelpersound.h"
#include "messageattachmentdelegatehelpertext.h"
#include "messageattachmentdelegatehelpervideo.h"
#include "messagedelegatehelperbase.h"
#include "messagedelegatehelperreactions.h"
#include "messagedelegatehelpertext.h"
#include "misc/avatarcachemanager.h"
#include "misc/emoticonmenuwidget.h"
#include "model/messagemodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
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

#include <cmath>
#include <tuple>

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
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::User, this))
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
}

MessageListDelegate::~MessageListDelegate()
{
    delete mTextSelectionImpl;
}

void MessageListDelegate::slotUpdateColors()
{
    KColorScheme scheme = Colors::self().schemeView();
    mThreadedMessageBackgroundColor = Colors::self().schemeWindow().background(KColorScheme::AlternateBackground).color();
    mOpenDiscussionColorMode = scheme.foreground(KColorScheme::LinkText).color();
    mReplyThreadColorMode = scheme.foreground(KColorScheme::NegativeText).color();
    updateView();
}

void MessageListDelegate::setRocketChatAccount(RocketChatAccount *rcAccount)
{
    mRocketChatAccount = rcAccount;
    mAvatarCacheManager->setCurrentRocketChatAccount(mRocketChatAccount);

    mHelperText->setRocketChatAccount(mRocketChatAccount);
    mHelperAttachmentImage->setRocketChatAccount(mRocketChatAccount);
    mHelperAttachmentFile->setRocketChatAccount(mRocketChatAccount);
    mHelperReactions->setRocketChatAccount(mRocketChatAccount);
    mHelperAttachmentVideo->setRocketChatAccount(mRocketChatAccount);
    mHelperAttachmentSound->setRocketChatAccount(mRocketChatAccount);
    mHelperAttachmentText->setRocketChatAccount(mRocketChatAccount);
}

QPixmap MessageListDelegate::makeAvatarPixmap(const QWidget *widget, const QModelIndex &index, int maxHeight) const
{
    const QString emojiStr = index.data(MessageModel::Emoji).toString();
    const auto info = index.data(MessageModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (emojiStr.isEmpty()) {
        const QString avatarUrl = index.data(MessageModel::Avatar).toString();
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

bool MessageListDelegate::showIgnoreMessages(const QModelIndex &index) const
{
    const bool isIgnoredMessage = index.data(MessageModel::Ignored).toBool();
    const bool isDirectMessage = index.data(MessageModel::DirectChannels).toBool();
    return isIgnoredMessage && !isDirectMessage;
}

// [Optional date header]
// [margin] <pixmap> [margin] <sender> [margin] <roles> [margin] <editicon> [margin] <favoriteicon> [margin] <text message> [margin] <add reaction> [margin]
// <timestamp> [margin/2]
//                                                                  <attachments>
//                                                                  <reactions>
//                                                                  <N replies>
MessageListDelegate::Layout MessageListDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    Q_ASSERT(message);
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);

    const auto sameSenderAsPreviousMessage = [&] {
        if (index.row() < 1) {
            return false;
        }

        const auto previousIndex = index.siblingAtRow(index.row() - 1);
        const auto previousMessage = previousIndex.data(MessageModel::MessagePointer).value<Message *>();
        Q_ASSERT(previousMessage);

        auto toTuple = [](const Message *message) {
            return std::make_tuple(message->userId(), message->threadMessageId(), QDateTime::fromMSecsSinceEpoch(message->timeStamp()).date());
        };
        return toTuple(message) == toTuple(previousMessage);
    }();

    Layout layout;
    if (mRocketChatAccount) {
        layout.senderText = QLatin1Char('@') + (mRocketChatAccount->useRealName() && !message->name().isEmpty() ? message->name() : message->username());
    } else {
        layout.senderText = QLatin1Char('@') + message->username();
    }
    layout.senderFont = option.font;
    layout.senderFont.setBold(true);
    layout.sameSenderAsPreviousMessage = sameSenderAsPreviousMessage;
    const QFontMetricsF senderFontMetrics(layout.senderFont);
    const qreal senderAscent = senderFontMetrics.ascent();
    const QSizeF senderTextSize = senderFontMetrics.size(Qt::TextSingleLine, layout.senderText);

    if (mRocketChatAccount && !mRocketChatAccount->hideAvatars()) {
        layout.avatarPixmap = makeAvatarPixmap(option.widget, index, senderTextSize.height());
    }

    QRect usableRect = option.rect;
    const bool displayLastSeenMessage = index.data(MessageModel::DisplayLastSeenMessage).toBool();
    if (index.data(MessageModel::DateDiffersFromPrevious).toBool()) {
        usableRect.setTop(usableRect.top() + option.fontMetrics.height());
    } else if (displayLastSeenMessage) {
        layout.displayLastSeenMessageY = usableRect.top();
    }

    layout.usableRect = usableRect; // Just for the top, for now. The left will move later on.

    const qreal margin = MessageDelegateUtils::basicMargin();
    const int senderX = option.rect.x() + MessageDelegateUtils::dprAwareSize(layout.avatarPixmap).width() + 2 * margin;
    int textLeft = senderX + senderTextSize.width() + margin;

    // Roles icon
    const bool hasRoles = !index.data(MessageModel::Roles).toString().isEmpty() && mRocketChatAccount && !mRocketChatAccount->hideRoles();
    if (hasRoles) {
        textLeft += iconSize + margin;
    }

    // Edit icon
    const int editIconX = textLeft;
    if (message->wasEdited()) {
        textLeft += iconSize + margin;
    }

    const int favoriteIconX = textLeft;
    // Favorite icon
    if (message->isStarred()) {
        textLeft += iconSize + margin;
    }

    const int pinIconX = textLeft;
    // Pin icon
    if (message->isPinned()) {
        textLeft += iconSize + margin;
    }

    const int followingIconX = textLeft;
    layout.messageIsFollowing = mRocketChatAccount && message->replies().contains(mRocketChatAccount->userId());
    // Following icon
    if (layout.messageIsFollowing) {
        textLeft += iconSize + margin;
    }

    const int translatedIconX = textLeft;
    // translated icon
    if (message->isAutoTranslated()) {
        textLeft += iconSize + margin;
    }

    const int showIgnoreMessageIconX = textLeft;
    // showIgnoreMessage icon
    const bool ignoreMessage = showIgnoreMessages(index);
    if (ignoreMessage) {
        textLeft += iconSize + margin;
    }

    // Timestamp
    layout.timeStampText = index.data(MessageModel::Timestamp).toString();
    const QSize timeSize = MessageDelegateUtils::timeStampSize(layout.timeStampText, option);

    // Message (using the rest of the available width)
    const int widthAfterMessage = iconSize + margin + timeSize.width() + margin / 2;
    const int maxWidth = qMax(30, option.rect.width() - textLeft - widthAfterMessage);
    layout.baseLine = 0;
    const QSize textSize = mHelperText->sizeHint(index, maxWidth, option, &layout.baseLine);
    int attachmentsY;
    const int textVMargin = 3; // adjust this for "compactness"
    if (textSize.isValid()) {
        layout.textRect = QRect(textLeft, usableRect.top() + textVMargin, maxWidth, textSize.height() + textVMargin);
        attachmentsY = layout.textRect.y() + layout.textRect.height();
        layout.baseLine += layout.textRect.top(); // make it absolute
    } else {
        attachmentsY = usableRect.top() + textVMargin;
        layout.baseLine = attachmentsY + option.fontMetrics.ascent();
    }
    layout.usableRect.setLeft(textLeft);

    // Align top of sender rect so it matches the baseline of the richtext
    layout.senderRect = QRectF(senderX, layout.baseLine - senderAscent, senderTextSize.width(), senderTextSize.height());
    // Align top of avatar with top of sender rect
    layout.avatarPos = QPointF(option.rect.x() + margin, layout.senderRect.y());
    // Same for the roles and edit icon
    if (hasRoles) {
        layout.rolesIconRect = QRect(editIconX - iconSize - margin, layout.senderRect.y(), iconSize, iconSize);
    }
    if (message->wasEdited()) {
        layout.editedIconRect = QRect(editIconX, layout.senderRect.y(), iconSize, iconSize);
    }

    if (message->isStarred()) {
        layout.favoriteIconRect = QRect(favoriteIconX, layout.senderRect.y(), iconSize, iconSize);
    }

    if (message->isPinned()) {
        layout.pinIconRect = QRect(pinIconX, layout.senderRect.y(), iconSize, iconSize);
    }
    if (layout.messageIsFollowing) {
        layout.followingIconRect = QRect(followingIconX, layout.senderRect.y(), iconSize, iconSize);
    }
    if (message->isAutoTranslated()) {
        layout.translatedIconRect = QRect(translatedIconX, layout.senderRect.y(), iconSize, iconSize);
    }

    if (ignoreMessage) {
        layout.showIgnoredMessageIconRect = QRect(showIgnoreMessageIconX, layout.senderRect.y(), iconSize, iconSize);
        layout.showIgnoreMessage = index.data(MessageModel::ShowIgnoredMessage).toBool();
    }

    layout.addReactionRect = QRect(textLeft + maxWidth, layout.senderRect.y(), iconSize, iconSize);
    layout.timeStampPos = QPoint(option.rect.width() - timeSize.width() - margin / 2, layout.baseLine);
    layout.timeStampRect = QRect(QPoint(layout.timeStampPos.x(), usableRect.top()), timeSize);

    if (message->attachments().isEmpty()) {
        layout.reactionsY = attachmentsY;
    } else {
        const auto attachments = message->attachments();
        QSize attachmentsSize;
        int topAttachment = attachmentsY;
        // TODO add spacing between attachment
        for (const MessageAttachment &msgAttach : attachments) {
            const MessageDelegateHelperBase *helper = attachmentsHelper(msgAttach);
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
        layout.reactionsY = attachmentsY + layout.attachmentsRect.height();
    }
    layout.reactionsHeight = mHelperReactions->sizeHint(index, maxWidth, option).height();

    // Replies
    layout.repliesY = layout.reactionsY + layout.reactionsHeight;
    if (message->threadCount() > 0) {
        layout.repliesHeight = option.fontMetrics.height();
    }
    // Discussions
    if (!message->discussionRoomId().isEmpty()) {
        layout.discussionsHeight = option.fontMetrics.height();
    }

    return layout;
}

MessageDelegateHelperBase *MessageListDelegate::attachmentsHelper(const MessageAttachment &msgAttach) const
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
        break;
    }
    return nullptr;
}

const QString &MessageListDelegate::searchText() const
{
    return mSearchText;
}

void MessageListDelegate::setSearchText(const QString &newSearchText)
{
    if (mSearchText != newSearchText) {
        mSearchText = newSearchText;
        mHelperText->setSearchText(mSearchText);
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

void MessageListDelegate::drawDate(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option, bool drawLastSeenLine) const
{
    const QPen origPen = painter->pen();
    const qreal margin = MessageDelegateUtils::basicMargin();
    const QString dateStr = index.data(MessageModel::Date).toString();
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

void MessageListDelegate::clearTextDocumentCache()
{
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
    const Layout layout = doLayout(option, index);
    const auto messageRect = layout.textRect;
    QString url = mHelperText->urlAt(index, pos - messageRect.topLeft());
    if (url.isEmpty()) {
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
        Q_ASSERT(message);
        const auto attachments = message->attachments();
        int i = 0;
        for (const MessageAttachment &msgAttach : attachments) {
            MessageDelegateHelperBase *helper = attachmentsHelper(msgAttach);
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
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    if (!message) {
        return false;
    }

    const Layout layout = doLayout(option, index);
    if (layout.senderRect.contains(info.pos)) {
        QMenu menu;
        auto userInfoAction = new QAction(QIcon::fromTheme(QStringLiteral("documentinfo")), i18n("User Info"), &menu);
        connect(userInfoAction, &QAction::triggered, this, [message, this]() {
            Q_EMIT showUserInfo(message->username());
        });

        if (info.editMode) {
            if (info.roomType != Room::RoomType::Direct) {
                menu.addSeparator();
                auto startPrivateConversationAction = new QAction(i18n("Start a Private Conversation"), &menu);
                connect(startPrivateConversationAction, &QAction::triggered, this, [=]() {
                    Q_EMIT startPrivateConversation(message->username());
                });
                menu.addAction(startPrivateConversationAction);
            }
        }

        menu.addSeparator();
        menu.addAction(userInfoAction);
        if (!menu.actions().isEmpty()) {
            menu.exec(info.globalPos);
        }
        return true;
    }
    return false;
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

void MessageListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    if (message->isEditingMode()) {
        painter->fillRect(option.rect, mEditColorMode);
    } else if (mHelperText->showThreadContext() && !message->threadMessageId().isEmpty()) {
        painter->fillRect(option.rect, mThreadedMessageBackgroundColor);
    } else {
        drawBackground(painter, option, index);
    }

    const Layout layout = doLayout(option, index);

    // Draw date if it differs from the previous message
    const bool displayLastSeenMessage = index.data(MessageModel::DisplayLastSeenMessage).toBool();
    if (index.data(MessageModel::DateDiffersFromPrevious).toBool()) {
        drawDate(painter, index, option, displayLastSeenMessage);
    } else if (displayLastSeenMessage) {
        drawLastSeenLine(painter, layout.displayLastSeenMessageY, option);
    }

    // Timestamp
    DelegatePaintUtil::drawLighterText(painter, layout.timeStampText, layout.timeStampPos);
    const Message::MessageType messageType = message->messageType();
    const bool isSystemMessage = (messageType == Message::System) || (messageType == Message::Information);
    if (!isSystemMessage) {
        mAddReactionIcon.paint(painter, layout.addReactionRect, Qt::AlignCenter);
    }

    // Message
    if (layout.textRect.isValid()) {
        mHelperText->draw(painter, layout.textRect, index, option);
    }

    // Draw the pixmap
    if (!mRocketChatAccount->hideAvatars() && !layout.sameSenderAsPreviousMessage) {
        painter->drawPixmap(layout.avatarPos, layout.avatarPixmap);
    }

    if (!layout.sameSenderAsPreviousMessage) {
        // Draw the sender
        const QFont oldFont = painter->font();
        painter->setFont(layout.senderFont);
        painter->drawText(layout.senderRect.x(), layout.baseLine, layout.senderText);
        painter->setFont(oldFont);

        // Draw the roles icon
        if (!index.data(MessageModel::Roles).toString().isEmpty() && !mRocketChatAccount->hideRoles()) {
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

    if (showIgnoreMessages(index)) {
        const QIcon hideShowIcon = QIcon::fromTheme(layout.showIgnoreMessage ? QStringLiteral("visibility") : QStringLiteral("hint"));
        hideShowIcon.paint(painter, layout.showIgnoredMessageIconRect);
    }

    // Attachments
    const auto attachments = message->attachments();
    int i = 0;
    for (const MessageAttachment &att : attachments) {
        const MessageDelegateHelperBase *helper = attachmentsHelper(att);
        if (helper) {
            helper->draw(att, painter, layout.attachmentsRectList.at(i), index, option);
        }
        ++i;
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

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Note: option.rect in this method is huge (as big as the viewport)
    const Layout layout = doLayout(option, index);

    int additionalHeight = 0;
    // A little bit of margin below the very last item, it just looks better
    if (index.row() == index.model()->rowCount() - 1) {
        additionalHeight += 4;
    }

    // contents is date + text + attachments + reactions + replies + discussions (where all of those are optional)
    const int contentsHeight = layout.repliesY + layout.repliesHeight + layout.discussionsHeight - option.rect.y();
    const int senderAndAvatarHeight = qMax<int>(layout.senderRect.y() + layout.senderRect.height() - option.rect.y(),
                                                layout.avatarPos.y() + MessageDelegateUtils::dprAwareSize(layout.avatarPixmap).height() - option.rect.y());

    // qDebug() << "senderAndAvatarHeight" << senderAndAvatarHeight << "text" << layout.textRect.height()
    //         << "attachments" << layout.attachmentsRect.height() << "reactions" << layout.reactionsHeight << "total contents" << contentsHeight;
    // qDebug() << "=> returning" << qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight;

    return {option.rect.width(), qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight};
}

static void positionPopup(QPoint pos, QWidget *parentWindow, QWidget *popup)
{
    const QRect screenRect = parentWindow->screen()->availableGeometry();

    const QSize popupSize{popup->sizeHint()};
    QRect popupRect(QPoint(pos.x() - popupSize.width(), pos.y() - popupSize.height()), popup->sizeHint());
    if (popupRect.top() < screenRect.top()) {
        popupRect.moveTop(screenRect.top());
    }
    popup->setGeometry(popupRect);
}

bool MessageListDelegate::mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto mev = static_cast<QMouseEvent *>(event);
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

        const Layout layout = doLayout(option, index);

        const Message::MessageType messageType = message->messageType();
        const bool isSystemMessage = (messageType == Message::System) || (messageType == Message::Information);
        if (layout.addReactionRect.contains(mev->pos()) && !isSystemMessage) {
            auto mEmoticonMenuWidget = new EmoticonMenuWidget(mListView);
            mEmoticonMenuWidget->setWindowFlag(Qt::Popup);
            mEmoticonMenuWidget->setCurrentRocketChatAccount(mRocketChatAccount);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            positionPopup(mev->globalPos(), mListView, mEmoticonMenuWidget);
#else
            positionPopup(mev->globalPosition().toPoint(), mListView, mEmoticonMenuWidget);
#endif
            mEmoticonMenuWidget->show();
            connect(mEmoticonMenuWidget, &EmoticonMenuWidget::insertEmoticons, this, [=](const QString &id) {
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
            const QRect threadRect(layout.usableRect.x(), layout.repliesY, layout.usableRect.width(), layout.repliesHeight);
            if (threadRect.contains(mev->pos())) {
                const QString threadMessagePreview = index.data(MessageModel::ThreadMessagePreview).toString();
                const bool threadIsFollowing = message->replies().contains(mRocketChatAccount->userId());
                Q_EMIT mRocketChatAccount->openThreadRequested(message->messageId(),
                                                               threadMessagePreview.isEmpty() ? index.data(MessageModel::MessageConvertedText).toString()
                                                                                              : threadMessagePreview,
                                                               threadIsFollowing);
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
        if (showIgnoreMessages(index)) {
            if (layout.showIgnoredMessageIconRect.contains(mev->pos())) {
                mHelperText->removeMessageCache(message->messageId());
                auto model = const_cast<QAbstractItemModel *>(index.model());
                model->setData(index, !layout.showIgnoreMessage, MessageModel::ShowIgnoredMessage);
                return true;
            }
        }
        if (mHelperText->handleMouseEvent(mev, layout.textRect, option, index)) {
            return true;
        }

        const auto attachments = message->attachments();
        int i = 0;
        for (const MessageAttachment &att : attachments) {
            MessageDelegateHelperBase *helper = attachmentsHelper(att);
            if (helper && helper->handleMouseEvent(att, mev, layout.attachmentsRectList.at(i), option, index)) {
                return true;
            }
            ++i;
        }
    } else if (eventType == QEvent::MouseButtonPress || eventType == QEvent::MouseMove || eventType == QEvent::MouseButtonDblClick) {
        auto mev = static_cast<QMouseEvent *>(event);
        if (mev->buttons() & Qt::LeftButton) {
            const Layout layout = doLayout(option, index);
            if (mHelperText->handleMouseEvent(mev, layout.textRect, option, index)) {
                return true;
            }

            const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
            const auto attachments = message->attachments();
            int i = 0;
            for (const MessageAttachment &att : attachments) {
                MessageDelegateHelperBase *helper = attachmentsHelper(att);
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
    const Layout layout = doLayout(option, index);
    if (mHelperText->maybeStartDrag(event, layout.textRect, option, index)) {
        return true;
    }

    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const auto attachments = message->attachments();
    int i = 0;
    for (const MessageAttachment &att : attachments) {
        MessageDelegateHelperBase *helper = attachmentsHelper(att);
        if (helper && helper->maybeStartDrag(att, event, layout.attachmentsRectList.at(i), option, index)) {
            return true;
        }
        ++i;
    }
    return false;
}

bool MessageListDelegate::helpEvent(QHelpEvent *helpEvent, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (helpEvent->type() == QEvent::ToolTip) {
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
        if (!message) {
            // tooltip was requested in an empty space below the last message, nothing to do
            return false;
        }

        const Layout layout = doLayout(option, index);
        if (!message->reactions().isEmpty()) {
            const QRect reactionsRect(layout.senderRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
            if (mHelperReactions->handleHelpEvent(helpEvent, view, reactionsRect, option, message)) {
                return true;
            }
        }
        const QPoint helpEventPos{helpEvent->pos()};
        if (layout.rolesIconRect.contains(helpEventPos)) {
            const QString tooltip = index.data(MessageModel::Roles).toString();
            QToolTip::showText(helpEvent->globalPos(), tooltip, view);
            return true;
        }
        if (layout.editedIconRect.contains(helpEventPos)) {
            const QString tooltip = index.data(MessageModel::EditedToolTip).toString();
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
            MessageDelegateHelperBase *helper = attachmentsHelper(att);
            if (helper) {
                if (layout.attachmentsRectList.at(i).contains(helpEventPos)
                    && helper->handleHelpEvent(helpEvent, layout.attachmentsRectList.at(i), att, option)) {
                    return true;
                }
            }
            ++i;
        }

        if (layout.timeStampRect.contains(helpEvent->pos())) {
            const QString dateStr = index.data(MessageModel::Date).toString();
            QToolTip::showText(helpEvent->globalPos(), dateStr, view);
            return true;
        }
    }
    return false;
}
