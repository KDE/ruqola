/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emojicustomdelegate.h"
#include "common/delegatepaintutil.h"
#include "emoticons/emojimanager.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "utils.h"
#include <QPainter>
#include <model/emoticonmodel.h>

EmojiCustomDelegate::EmojiCustomDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mEmojiFont(Utils::emojiFontName())
{
}

EmojiCustomDelegate::~EmojiCustomDelegate() = default;

void EmojiCustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    drawBackground(painter, option, index);
    const int margin = DelegatePaintUtil::margin();

    const QString text = index.data(EmoticonModel::CompleterName).toString();
    const int iconSize = 15; // TODO fix icon size
    auto rcAccount = Ruqola::self()->rocketChatAccount();
    if (rcAccount) {
        const QString fileName = rcAccount->emojiManager()->customEmojiFileName(text);
        if (!fileName.isEmpty()) {
            const QUrl emojiUrl = rcAccount->attachmentUrlFromLocalCache(fileName);
            if (!emojiUrl.isEmpty()) {
                const QIcon icon(emojiUrl.toLocalFile());
                icon.paint(painter, option.rect, Qt::AlignLeft);
            }
        }
    }

    const int xText = option.rect.x() + margin + iconSize;
    const QRect displayRect(xText, option.rect.y(), option.rect.width() - xText, option.rect.height());

    drawDisplay(painter, option, displayRect, text);
}
