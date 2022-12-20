/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselection.h"
#include "delegateutils/textselectionimpl.h"
#include "messageblockdelegatehelperbase.h"
#include "messages/block.h"

#include <QIcon>
#include <QModelIndex>
#include <QSize>
#include <QTextDocument>
#include <lrucache.h>

#include <memory>

class QListView;
class QPainter;
class QRect;
class QMouseEvent;
class QHelpEvent;
class QStyleOptionViewItem;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperConferenceVideo : public MessageBlockDelegateHelperBase
{
public:
    explicit MessageDelegateHelperConferenceVideo(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageDelegateHelperConferenceVideo() override;
    void draw(const Block &block, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    Q_REQUIRED_RESULT QSize sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;
    Q_REQUIRED_RESULT bool
    handleMouseEvent(const Block &block, QMouseEvent *mouseEvent, QRect attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    struct ConferenceCallLayout {
        QString description;
        QSize descriptionSize;
        QString title;
        QRectF titleRect;
    };
    Q_REQUIRED_RESULT QPoint adaptMousePosition(const QPoint &pos, const Block &block, QRect attachmentsRect, const QStyleOptionViewItem &option);
    Q_REQUIRED_RESULT ConferenceCallLayout layoutConferenceCall(const Block &block, const QStyleOptionViewItem &option, int attachmentsWidth) const;

    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const QModelIndex &index) const override;
    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const MessageAttachment &msgAttach) const override;
    Q_REQUIRED_RESULT QTextDocument *documentForIndex(const Block &block) const override;

    const QIcon mInfoIcon;
};
