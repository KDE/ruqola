/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselectionimpl.h"
#include "messageblockdelegatehelperbase.h"
#include "messages/block.h"

#include <QIcon>
#include <QModelIndex>
#include <QSize>
class QTextDocument;
#include <lrucache.h>

#include <memory>

class QListView;
class QPainter;
class QRect;
class QMouseEvent;
class QStyleOptionViewItem;
class RocketChatAccount;
class AvatarCacheManager;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperConferenceVideo : public MessageBlockDelegateHelperBase
{
public:
    explicit MessageDelegateHelperConferenceVideo(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageDelegateHelperConferenceVideo() override;
    void draw(const Block &block, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] QSize sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] bool
    handleMouseEvent(const Block &block, QMouseEvent *mouseEvent, QRect blockRect, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    [[nodiscard]] bool handleHelpEvent(QHelpEvent *helpEvent, QRect blockRect, const Block &block, const QStyleOptionViewItem &option) override;

    void setRocketChatAccount(RocketChatAccount *newRocketChatAccount) override;

private:
    struct UserLayout {
        QRectF userAvatarRect;
        QString userName;
        QPixmap avatarPixmap;
    };
    struct ConferenceCallLayout {
        QVector<UserLayout> usersLayout;
        QString title;
        QSize titleSize;
        QRect infoButtonRect;
        QString joinButtonText;
        QSize joinButtonTextSize;
        QRect joinButtonRect;
        bool canJoin = false;
    };
    [[nodiscard]] QPoint adaptMousePosition(const QPoint &pos, const Block &block, QRect attachmentsRect, const QStyleOptionViewItem &option);
    [[nodiscard]] ConferenceCallLayout layoutConferenceCall(const Block &block, const QStyleOptionViewItem &option, int attachmentsWidth) const;

    [[nodiscard]] QTextDocument *documentForIndex(const QModelIndex &index) const override;
    [[nodiscard]] QTextDocument *documentForIndex(const MessageAttachment &msgAttach) const override;
    [[nodiscard]] QTextDocument *documentForIndex(const Block &block) const override;
    [[nodiscard]] QPixmap makeAvatarPixmap(const QString &identifier, const QWidget *widget, int maxHeight) const;

    const QIcon mInfoIcon;
    AvatarCacheManager *const mAvatarCacheManager;
};
