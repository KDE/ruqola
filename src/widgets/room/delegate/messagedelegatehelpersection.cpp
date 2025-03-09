/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelpersection.h"
#include "common/delegatepaintutil.h"
#include "rocketchataccount.h"
// #include "textconverter.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

MessageDelegateHelperSection::MessageDelegateHelperSection(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageBlockDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageDelegateHelperSection::~MessageDelegateHelperSection() = default;

void MessageDelegateHelperSection::draw(const Block &block,
                                        QPainter *painter,
                                        QRect blockRect,
                                        const QModelIndex &index,
                                        const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const SectionLayout layout = layoutSection(block, option, blockRect.width());
    // Draw title and buttons
    const int positionY = blockRect.y() + option.fontMetrics.ascent();
    painter->drawText(blockRect.x(), positionY, layout.sectionText);
    // DrawButton
    if (!layout.buttonText.isEmpty()) {
        const int positionX = blockRect.x() + layout.sectionTextSize.width() + DelegatePaintUtil::margin();
        const QRectF joinButtonRect = layout.buttonRect.translated(positionX, blockRect.y());
        painter->drawRoundedRect(joinButtonRect.adjusted(-5, 0, +5, 0), 5, 5);
        // Fix position
        painter->drawText(positionX, positionY, layout.buttonText);
    }
}

QSize MessageDelegateHelperSection::sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const SectionLayout layout = layoutSection(block, option, maxWidth);
    const int height = layout.sectionTextSize.height() + DelegatePaintUtil::margin();
    return {qMax(0, layout.sectionTextSize.width()), height};
}

bool MessageDelegateHelperSection::handleMouseEvent(const Block &block,
                                                    QMouseEvent *mouseEvent,
                                                    QRect blocksRect,
                                                    const QStyleOptionViewItem &option,
                                                    const QModelIndex &index)
{
    Q_UNUSED(block);
    Q_UNUSED(mouseEvent);
    Q_UNUSED(blocksRect);
    Q_UNUSED(option);
    Q_UNUSED(index);
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const QPoint pos = mouseEvent->pos();
        // if (!layout.buttonText.isEmpty()) {
        //  TODO
        //}
        // TODO
    }
    return false;
}

MessageDelegateHelperSection::SectionLayout
MessageDelegateHelperSection::layoutSection(const Block &block, const QStyleOptionViewItem &option, int blockRectWidth) const
{
    Q_UNUSED(blockRectWidth)
    SectionLayout layout;
#if 0 // We need to use QTextDocument here too
    auto emojiManager = mRocketChatAccount ? mRocketChatAccount->emojiManager() : nullptr;
    auto messageCache = mRocketChatAccount ? mRocketChatAccount->messageCache() : nullptr;
    QByteArray needUpdateMessageId;
    const TextConverter::ConvertMessageTextSettings settings(block.sectionText(), {}, {}, {}, emojiManager, messageCache, {}, {});
    int recursiveIndex = 0;
    layout.title = TextConverter::convertMessageText(settings, needUpdateMessageId, recursiveIndex);
#else
    layout.sectionText = block.sectionText();
    layout.sectionTextSize = option.fontMetrics.size(Qt::TextSingleLine, layout.sectionText);
    const auto blockAccessory = block.blockAccessory();
    switch (blockAccessory.type()) {
    case BlockAccessory::AccessoryType::Overflow: {
        if (!blockAccessory.options().isEmpty()) { }
        break;
    }
    case BlockAccessory::AccessoryType::Button: {
        layout.buttonText = blockAccessory.text();
        layout.buttonTextSize = option.fontMetrics.size(Qt::TextSingleLine, layout.buttonText);
        layout.buttonRect = QRect(0, 0, layout.buttonTextSize.width(), layout.buttonTextSize.height());
        break;
    }
    case BlockAccessory::AccessoryType::Unknown:
        break;
    }

#endif
    return layout;
}
