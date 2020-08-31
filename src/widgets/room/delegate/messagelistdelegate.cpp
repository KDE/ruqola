/*
   Copyright (c) 2020 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "messagelistdelegate.h"
#include "messagedelegatehelperbase.h"
#include "messagedelegatehelpertext.h"
#include "messagedelegatehelperimage.h"
#include "messagedelegatehelperfile.h"
#include "messagedelegatehelperreactions.h"
#include "messagedelegatehelpervideo.h"
#include "messagedelegatehelpersound.h"
#include "model/messagemodel.h"
#include "emoticons/emojimanager.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "rocketchataccount.h"
#include "misc/emoticonmenuwidget.h"
#include "common/delegatepaintutil.h"

#include <QApplication>
#include <QAbstractItemView>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QToolTip>

#include <KLocalizedString>
#include <KColorScheme>

static QSizeF dprAwareSize(const QPixmap &pixmap)
{
    if (pixmap.isNull()) {
        return {0, 0}; // prevent division-by-zero
    }
    return pixmap.size() / pixmap.devicePixelRatioF();
}

MessageListDelegate::MessageListDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mEmojiFont(QStringLiteral("NotoColorEmoji"))
    , mEditedIcon(QIcon::fromTheme(QStringLiteral("document-edit")))
    , mRolesIcon(QIcon::fromTheme(QStringLiteral("documentinfo")))
    // https://bugs.kde.org/show_bug.cgi?id=417298 added smiley-add to KF 5.68
    , mAddReactionIcon(QIcon::fromTheme(QStringLiteral("smiley-add"), QIcon::fromTheme(QStringLiteral("face-smile"))))
    , mHelperText(new MessageDelegateHelperText)
    , mHelperImage(new MessageDelegateHelperImage)
    , mHelperFile(new MessageDelegateHelperFile)
    , mHelperReactions(new MessageDelegateHelperReactions)
    , mHelperVideo(new MessageDelegateHelperVideo)
    , mHelperSound(new MessageDelegateHelperSound)
{
}

MessageListDelegate::~MessageListDelegate()
{
}

void MessageListDelegate::setRocketChatAccount(RocketChatAccount *rcAccount)
{
    mRocketChatAccount = rcAccount;
}

static qreal basicMargin()
{
    return 8;
}

static QSize timeStampSize(const QString &timeStampText, const QStyleOptionViewItem &option)
{
    // This gives incorrect results (too small bounding rect), no idea why!
    //const QSize timeSize = painter->fontMetrics().boundingRect(timeStampText).size();
    return QSize(option.fontMetrics.horizontalAdvance(timeStampText), option.fontMetrics.height());
}

QPixmap MessageListDelegate::makeAvatarUrlPixmap(const QWidget *widget, const QModelIndex &index, int maxHeight) const
{
    const QString userId = index.data(MessageModel::UserId).toString();
    const QString iconUrlStr = mRocketChatAccount->avatarUrl(userId);
    if (iconUrlStr.isEmpty()) {
        return {};
    }

    const auto dpr = widget->devicePixelRatioF();
    if (dpr != mAvatarCache.dpr) {
        mAvatarCache.dpr = dpr;
        mAvatarCache.cache.clear();
    }

    auto &cache = mAvatarCache.cache;

    auto downScaled = cache.findCachedPixmap(iconUrlStr);
    if (downScaled.isNull()) {
        const QUrl iconUrl(iconUrlStr);
        Q_ASSERT(iconUrl.isLocalFile());
        QPixmap fullScale;
        if (!fullScale.load(iconUrl.toLocalFile())) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Could not load" << iconUrl.toLocalFile();
            return {};
        }
        downScaled = fullScale.scaledToHeight(maxHeight * dpr, Qt::SmoothTransformation);
        downScaled.setDevicePixelRatio(dpr);
        cache.insertCachedPixmap(iconUrlStr, downScaled);
    }
    return downScaled;
}

QPixmap MessageListDelegate::makeAvatarEmojiPixmap(const QString &emojiStr, const QWidget *widget, const QModelIndex &index, int maxHeight) const
{
    const auto dpr = widget->devicePixelRatioF();
    if (dpr != mAvatarCache.dpr) {
        mAvatarCache.dpr = dpr;
        mAvatarCache.cache.clear();
    }

    auto &cache = mAvatarCache.cache;

    auto downScaled = cache.findCachedPixmap(emojiStr);
    if (downScaled.isNull()) {
        auto *emojiManager = mRocketChatAccount->emojiManager();
        const UnicodeEmoticon emoticon = emojiManager->unicodeEmoticonForEmoji(emojiStr);
        if (emoticon.isValid()) {
            const QFontMetrics fm(mEmojiFont);
            const QSize size = fm.boundingRect(emoticon.unicode()).size();

            //qDebug() << " size " << size << "emojiStr "<< emojiStr << " emoticon.unicode() " <<emoticon.unicode() << fm.horizontalAdvance(emoticon.unicode());
            //boundingRect can return a width == 0 for existing character as :warning: emoji.
            QPixmap fullScale(fm.horizontalAdvance(emoticon.unicode()), size.height());

            fullScale.fill(Qt::white);
            QPainter painter( &fullScale );
            painter.setFont(mEmojiFont);
            painter.drawText(fullScale.rect(), Qt::AlignCenter, emoticon.unicode());
            downScaled = fullScale.scaledToHeight(maxHeight * dpr, Qt::SmoothTransformation);
            downScaled.setDevicePixelRatio(dpr);
            cache.insertCachedPixmap(emojiStr, downScaled);
        } else {
            return makeAvatarUrlPixmap(widget, index, maxHeight);
        }
    }
    return downScaled;
}

QPixmap MessageListDelegate::makeAvatarPixmap(const QWidget *widget, const QModelIndex &index, int maxHeight) const
{
    const QString emojiStr = index.data(MessageModel::Emoji).toString();
    if (!emojiStr.isEmpty()) {
        return makeAvatarEmojiPixmap(emojiStr, widget, index, maxHeight);
    } else {
        return makeAvatarUrlPixmap(widget, index, maxHeight);
    }
}

// [Optional date header]
// [margin] <pixmap> [margin] <sender> [margin] <editicon> [margin] <text message> [margin] <add reaction> [margin] <timestamp> [margin/2]
//                                                                  <attachments>
//                                                                  <reactions>
//                                                                  <N replies>
MessageListDelegate::Layout MessageListDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    Q_ASSERT(message);
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);

    Layout layout;
    layout.senderText = QLatin1Char('@') + message->username();
    layout.senderFont = option.font;
    layout.senderFont.setBold(true);
    const QFontMetricsF senderFontMetrics(layout.senderFont);
    const qreal senderAscent = senderFontMetrics.ascent();
    const QSizeF senderTextSize = senderFontMetrics.size(Qt::TextSingleLine, layout.senderText);

    layout.avatarPixmap = makeAvatarPixmap(option.widget, index, senderTextSize.height());

    QRect usableRect = option.rect;
    const bool displayLastSeenMessage = index.data(MessageModel::DisplayLastSeenMessage).toBool();
    if (index.data(MessageModel::DateDiffersFromPrevious).toBool()) {
        usableRect.setTop(usableRect.top() + option.fontMetrics.height());
    } else if (displayLastSeenMessage) {
        layout.displayLastSeenMessageY = usableRect.top();
    }

    layout.usableRect = usableRect; // Just for the top, for now. The left will move later on.

    const qreal margin = basicMargin();
    const int senderX = option.rect.x() + dprAwareSize(layout.avatarPixmap).width() + 2 * margin;
    int textLeft = senderX + senderTextSize.width() + margin;

    // Roles icon
    const bool hasRoles = !index.data(MessageModel::Roles).toString().isEmpty();
    if (hasRoles) {
        textLeft += iconSize + margin;
    }

    // Edit icon
    const int editIconX = textLeft;
    if (message->wasEdited()) {
        textLeft += iconSize + margin;
    }

    // Timestamp
    layout.timeStampText = index.data(MessageModel::Timestamp).toString();
    const QSize timeSize = timeStampSize(layout.timeStampText, option);

    // Message (using the rest of the available width)
    const int widthAfterMessage = iconSize + margin + timeSize.width() + margin / 2;
    const int maxWidth = qMax(30, option.rect.width() - textLeft - widthAfterMessage);
    layout.baseLine = 0;
    const QSize textSize = mHelperText->sizeHint(index, maxWidth, option, &layout.baseLine);
    int attachmentsY;
    const int textVMargin = 3; // adjust this for "compactness"
    if (textSize.isValid()) {
        layout.textRect = QRect(textLeft, usableRect.top() + textVMargin,
                                maxWidth, textSize.height() + textVMargin);
        attachmentsY = layout.textRect.y() + layout.textRect.height();
        layout.baseLine += layout.textRect.top(); // make it absolute
    } else {
        attachmentsY = usableRect.top() + textVMargin;
        layout.baseLine = attachmentsY + option.fontMetrics.ascent();
    }
    layout.usableRect.setLeft(textLeft);

    // Align top of sender rect so it matches the baseline of the richtext
    layout.senderRect = QRectF(senderX, layout.baseLine - senderAscent,
                               senderTextSize.width(), senderTextSize.height());
    // Align top of avatar with top of sender rect
    layout.avatarPos = QPointF(option.rect.x() + margin, layout.senderRect.y());
    // Same for the roles and edit icon
    if (hasRoles) {
        layout.rolesIconRect = QRect(editIconX - iconSize - margin, layout.senderRect.y(), iconSize, iconSize);
    }
    if (message->wasEdited()) {
        layout.editedIconRect = QRect(editIconX, layout.senderRect.y(), iconSize, iconSize);
    }

    layout.addReactionRect = QRect(textLeft + maxWidth, layout.senderRect.y(), iconSize, iconSize);
    layout.timeStampPos = QPoint(option.rect.width() - timeSize.width() - margin / 2, layout.baseLine);
    layout.timeStampRect = QRect(QPoint(layout.timeStampPos.x(), usableRect.top()), timeSize);

    if (!message->attachements().isEmpty()) {
        const MessageDelegateHelperBase *helper = attachmentsHelper(message);
        const QSize attachmentsSize = helper ? helper->sizeHint(index, maxWidth, option) : QSize(0, 0);
        layout.attachmentsRect = QRect(textLeft, attachmentsY, attachmentsSize.width(), attachmentsSize.height());
        layout.reactionsY = attachmentsY + attachmentsSize.height();
    } else {
        layout.reactionsY = attachmentsY;
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

MessageDelegateHelperBase *MessageListDelegate::attachmentsHelper(const Message *message) const
{
    switch (message->messageType()) {
    case Message::Image:
        return mHelperImage.data();
    case Message::File:
        return mHelperFile.data();
    case Message::Video:
        return mHelperVideo.data();
    case Message::Audio:
        return mHelperSound.data();
    case Message::System:
    case Message::Information:
    case Message::NormalText:
        break;
    }
    return nullptr;
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
    const qreal margin = basicMargin();
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
    const Layout layout = doLayout(option, index);
    mHelperText->selectAll(option.widget, layout.textRect, index);
}

QString MessageListDelegate::selectedText() const
{
    return mHelperText->selectedText();
}

bool MessageListDelegate::hasSelection() const
{
    return mHelperText->hasSelection();
}

void MessageListDelegate::setShowThreadContext(bool b)
{
    mHelperText->setShowThreadContext(b);
}

void MessageListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    drawBackground(painter, option, index);

    const Layout layout = doLayout(option, index);

    // Draw date if it differs from the previous message
    const bool displayLastSeenMessage = index.data(MessageModel::DisplayLastSeenMessage).toBool();
    if (index.data(MessageModel::DateDiffersFromPrevious).toBool()) {
        drawDate(painter, index, option, displayLastSeenMessage);
    } else if (displayLastSeenMessage) {
        drawLastSeenLine(painter, layout.displayLastSeenMessageY, option);
    }

    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    // Timestamp
    DelegatePaintUtil::drawTimestamp(painter, layout.timeStampText, layout.timeStampPos);
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
    painter->drawPixmap(layout.avatarPos, layout.avatarPixmap);

    // Draw the sender
    const QFont oldFont = painter->font();
    painter->setFont(layout.senderFont);
    painter->drawText(layout.senderRect.x(), layout.baseLine, layout.senderText);
    painter->setFont(oldFont);

    // Draw the roles icon
    if (!index.data(MessageModel::Roles).toString().isEmpty()) {
        mRolesIcon.paint(painter, layout.rolesIconRect);
    }

    // Draw the edited icon
    if (message->wasEdited()) {
        mEditedIcon.paint(painter, layout.editedIconRect);
    }

    // Attachments
    const MessageDelegateHelperBase *helper = attachmentsHelper(message);
    if (helper) {
        helper->draw(painter, layout.attachmentsRect, index, option);
    }

    // Reactions
    const QRect reactionsRect(layout.usableRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
    mHelperReactions->draw(painter, reactionsRect, index, option);

    // Replies
    KColorScheme scheme;
    const int threadCount = message->threadCount();
    if (threadCount > 0) {
        const QString repliesText = i18np("1 reply", "%1 replies", threadCount);
        painter->setPen(scheme.foreground(KColorScheme::NegativeText).color());
        painter->drawText(layout.usableRect.x(), layout.repliesY + option.fontMetrics.ascent(), repliesText);
    }
    // Discussion
    if (!message->discussionRoomId().isEmpty()) {
        const QString discussionsText = (message->discussionCount() > 0) ? i18np("1 message", "%1 messages", message->discussionCount()) : i18n("No message yet");
        painter->setPen(scheme.foreground(KColorScheme::LinkText).color());
        painter->drawText(layout.usableRect.x(), layout.repliesY + layout.repliesHeight + option.fontMetrics.ascent(), discussionsText);
        // Note: pen still blue, currently relying on restore()
    }

    //drawFocus(painter, option, messageRect);

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
                                                layout.avatarPos.y() + dprAwareSize(layout.avatarPixmap).height() - option.rect.y());

    //qDebug() << "senderAndAvatarHeight" << senderAndAvatarHeight << "text" << layout.textRect.height()
    //         << "attachments" << layout.attachmentsRect.height() << "reactions" << layout.reactionsHeight << "total contents" << contentsHeight;
    //qDebug() << "=> returning" << qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight;

    return QSize(option.rect.width(),
                 qMax(senderAndAvatarHeight, contentsHeight) + additionalHeight);
}

static void positionPopup(QPoint pos, QWidget *parentWindow, QWidget *popup)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    const QRect screenRect = parentWindow->screen()->availableGeometry();
#else
    const int screenNum = QApplication::desktop()->screenNumber(parentWindow);
    auto *screen = QApplication::screens().value(screenNum);
    Q_ASSERT(screen);
    const QRect screenRect = screen->availableGeometry();
#endif

    QRect popupRect(pos, popup->sizeHint());
    if (popupRect.width() > screenRect.width()) {
        popupRect.setWidth(screenRect.width());
    }
    if (popupRect.right() > screenRect.right()) {
        popupRect.moveRight(screenRect.right());
    }
    if (popupRect.top() < screenRect.top()) {
        popupRect.moveTop(screenRect.top());
    }
    if (popupRect.bottom() > screenRect.bottom()) {
        popupRect.moveBottom(screenRect.bottom());
    }
    popup->setGeometry(popupRect);
}

bool MessageListDelegate::mouseEvent(QEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto *mev = static_cast<QMouseEvent *>(event);
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

        const Layout layout = doLayout(option, index);

        const Message::MessageType messageType = message->messageType();
        const bool isSystemMessage = (messageType == Message::System) || (messageType == Message::Information);
        if (layout.addReactionRect.contains(mev->pos()) && !isSystemMessage) {
            QWidget *parentWidget = const_cast<QWidget *>(option.widget);
            EmoticonMenuWidget *mEmoticonMenuWidget = new EmoticonMenuWidget(parentWidget);
            mEmoticonMenuWidget->setWindowFlag(Qt::Popup);
            mEmoticonMenuWidget->setCurrentRocketChatAccount(mRocketChatAccount);
            positionPopup(mev->globalPos(), parentWidget, mEmoticonMenuWidget);
            mEmoticonMenuWidget->show();
            connect(mEmoticonMenuWidget, &EmoticonMenuWidget::insertEmoticons, this, [=](const QString &id) {
                mRocketChatAccount->reactOnMessage(message->messageId(), id, true /*add*/);
            });
            return true;
        }

        if (!message->reactions().isEmpty()) {
            const QRect reactionsRect(layout.usableRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
            if (mHelperReactions->handleMouseEvent(mev, reactionsRect, option, message)) {
                return true;
            }
        }
        if (message->threadCount() > 0) {
            const QRect threadRect(layout.usableRect.x(), layout.repliesY, layout.usableRect.width(), layout.repliesHeight);
            if (threadRect.contains(mev->pos())) {
                const QString threadMessagePreview = index.data(MessageModel::ThreadMessagePreview).toString();
                Q_EMIT mRocketChatAccount->openThreadRequested(message->messageId(), threadMessagePreview.isEmpty() ? index.data(MessageModel::MessageConvertedText).toString() : threadMessagePreview);
                return true;
            }
        }
        if (!message->discussionRoomId().isEmpty()) {
            const QRect discussionRect(layout.usableRect.x(), layout.repliesY + layout.repliesHeight, layout.usableRect.width(), layout.discussionsHeight);
            if (discussionRect.contains(mev->pos())) {
                qDebug() << " Open discussion" << message->discussionRoomId();
                // We need to fix rest api first
                mRocketChatAccount->joinDiscussion(message->discussionRoomId(), QString());
                return true;
            }
        }
        if (mHelperText->handleMouseEvent(mev, layout.textRect, option, index)) {
            return true;
        }

        MessageDelegateHelperBase *helper = attachmentsHelper(message);
        if (helper && helper->handleMouseEvent(mev, layout.attachmentsRect, option, index)) {
            return true;
        }
    } else if (eventType == QEvent::MouseButtonPress || eventType == QEvent::MouseMove || eventType == QEvent::MouseButtonDblClick) {
        auto *mev = static_cast<QMouseEvent *>(event);
        if (mev->buttons() & Qt::LeftButton) {
            const Layout layout = doLayout(option, index);
            if (mHelperText->handleMouseEvent(mev, layout.textRect, option, index)) {
                return true;
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
    // TODO: dragging images/movies/...
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
            const QRect reactionsRect(layout.usableRect.x(), layout.reactionsY, layout.usableRect.width(), layout.reactionsHeight);
            if (mHelperReactions->handleHelpEvent(helpEvent, view, reactionsRect, option, message)) {
                return true;
            }
        }
        if (layout.rolesIconRect.contains(helpEvent->pos())) {
            const QString tooltip = index.data(MessageModel::Roles).toString();
            QToolTip::showText(helpEvent->globalPos(), tooltip, view);
            return true;
        }
        if (layout.textRect.contains(helpEvent->pos()) && mHelperText->handleHelpEvent(helpEvent, view, layout.textRect, index)) {
            return true;
        }
        if (layout.timeStampRect.contains(helpEvent->pos())) {
            const QString dateStr = index.data(MessageModel::Date).toString();
            QToolTip::showText(helpEvent->globalPos(), dateStr, view);
            return true;
        }
    }
    return false;
}
