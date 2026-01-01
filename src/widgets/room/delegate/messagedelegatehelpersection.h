/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselectionimpl.h"
#include "messageblockdelegatehelperbase.h"
#include "messages/block/block.h"

#include <QIcon>
#include <QModelIndex>
#include <QSize>

class QListView;
class QPainter;
class QRect;
class QMouseEvent;
class QStyleOptionViewItem;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperSection : public MessageBlockDelegateHelperBase
{
public:
    explicit MessageDelegateHelperSection(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageDelegateHelperSection() override;
    void draw(const Block &block, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const override;
    [[nodiscard]] QSize sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const override;

    [[nodiscard]] bool
    handleMouseEvent(const Block &block, QMouseEvent *mouseEvent, QRect blocksRect, const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    struct SectionLayout {
        QString sectionText;
        QSize sectionTextSize;

        // button
        QString buttonText;
        QSize buttonTextSize;
        QRect buttonRect;

        // Menu
        QRect menuRect;
    };
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT SectionLayout layoutSection(const Block &block,
                                                                         const QStyleOptionViewItem &option,
                                                                         int attachmentsWidth = -1) const;

    const QIcon mMenuIcon;
};
