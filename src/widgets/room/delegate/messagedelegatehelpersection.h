/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselectionimpl.h"
#include "messageblockdelegatehelperbase.h"
#include "messages/block.h"

#include <QIcon>
#include <QModelIndex>
#include <QSize>

class QListView;
class QPainter;
class QRect;
class QMouseEvent;
class QStyleOptionViewItem;
class RocketChatAccount;
class AvatarCacheManager;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperSection : public MessageBlockDelegateHelperBase
{
public:
    explicit MessageDelegateHelperSection(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageDelegateHelperSection() override;
    void draw(const Block &block, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] QSize sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;

    [[nodiscard]] bool handleHelpEvent(QHelpEvent *helpEvent, QRect blockRect, const Block &block, const QStyleOptionViewItem &option) override;

    [[nodiscard]] bool
    handleMouseEvent(const Block &block, QMouseEvent *mouseEvent, QRect blocksRect, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount) override;

private:
    struct UserLayout {
        QRectF userAvatarRect;
        QString userName;
        QPixmap avatarPixmap;
    };
    struct ConferenceCallLayout {
        QList<UserLayout> usersLayout;
        QString title;
        QSize titleSize;
        QRect infoButtonRect;
        QSize joinButtonTextSize;
        QRect joinButtonRect;
        bool canJoin = false;
    };
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QPoint adaptMousePosition(const QPoint &pos,
                                                                       const Block &block,
                                                                       QRect attachmentsRect,
                                                                       const QStyleOptionViewItem &option);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT ConferenceCallLayout layoutConferenceCall(const Block &block,
                                                                                       const QStyleOptionViewItem &option,
                                                                                       int attachmentsWidth) const;

    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QPixmap makeAvatarPixmap(const QString &identifier, const QWidget *widget, int maxHeight) const;

    const QIcon mInfoIcon;
    AvatarCacheManager *const mAvatarCacheManager;
};
