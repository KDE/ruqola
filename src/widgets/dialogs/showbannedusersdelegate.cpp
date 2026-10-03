/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showbannedusersdelegate.h"
#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/bannedusersmodel.h"
#include "utils.h"
#include <QPainter>

using namespace Qt::Literals::StringLiterals;
ShowBannedUsersDelegate::ShowBannedUsersDelegate(QObject *parent)
    : QItemDelegate{parent}
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::User, this))
{
}

ShowBannedUsersDelegate::~ShowBannedUsersDelegate() = default;

void ShowBannedUsersDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] icon ? name (username)
    drawBackground(painter, option, index);

    if (!option.showDecorationSelected && (option.state & QStyle::State_Selected)) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    const int margin = DelegatePaintUtil::margin();
    const QFont oldFont = painter->font();
    const QPen oldPen = painter->pen();
    // The text is painted with QPainter::drawText(), so unlike drawDisplay() it doesn't switch
    // to QPalette::HighlightedText by itself.
    DelegatePaintUtil::setTextPen(painter, option);

    QFont boldFont = oldFont;
    boldFont.setBold(true);
    painter->setFont(boldFont);

    const QFontMetrics fontMetrics(boldFont);
    const int defaultCharHeight = option.rect.y() + (option.rect.height() - fontMetrics.height()) / 2 + fontMetrics.ascent();
    const int rightLimit = option.rect.right() - margin;

    int xPos = option.rect.x();
    const Utils::AvatarInfo info = index.data(BannedUsersModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const int iconSize = option.rect.height() - 4;
        const int iconY = option.rect.y() + 2;
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(option.widget, info, iconSize);
        if (!pix.isNull()) {
            painter->drawPixmap(xPos + margin, iconY, iconSize, iconSize, pix);
        }
        // Add extra size even if we don't have avatar pix
        xPos += margin + iconSize;
    }
    xPos += margin;

    const QString name = index.data(BannedUsersModel::Name).toString();
    const QString userName = index.data(BannedUsersModel::UserName).toString();
    if (name.isEmpty()) {
        painter->drawText(xPos, defaultCharHeight, fontMetrics.elidedText(userName, Qt::ElideRight, rightLimit - xPos));
    } else {
        const QString elidedName = fontMetrics.elidedText(name, Qt::ElideRight, rightLimit - xPos);
        painter->drawText(xPos, defaultCharHeight, elidedName);
        painter->setFont(oldFont);
        xPos += fontMetrics.horizontalAdvance(name) + margin;
        // Only when the name was drawn in full is there room left for the username
        if (!userName.isEmpty() && elidedName == name && xPos < rightLimit) {
            const QString userNameStr = u"(%1)"_s.arg(userName);
            DelegatePaintUtil::drawLighterText(painter,
                                               option.fontMetrics.elidedText(userNameStr, Qt::ElideRight, rightLimit - xPos),
                                               QPoint(xPos, defaultCharHeight));
        }
    }
    painter->setFont(oldFont);
    painter->setPen(oldPen);
}

void ShowBannedUsersDelegate::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(newRocketChatAccount);
}

QSize ShowBannedUsersDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QSize size = QItemDelegate::sizeHint(option, index);
    const int margin = DelegatePaintUtil::margin();
    const int height = size.height() + 4;
    // QItemDelegate only measured the display role (the name). Account for what paint()
    // draws around it: the avatar and the "(username)" suffix.
    int width = size.width() + margin;
    if (index.data(BannedUsersModel::AvatarInfo).value<Utils::AvatarInfo>().isValid()) {
        width += margin + height - 4;
    }
    if (const QString userName = index.data(BannedUsersModel::UserName).toString(); !userName.isEmpty()) {
        if (index.data(BannedUsersModel::Name).toString().isEmpty()) {
            // No name: paint() draws the username in bold in its place
            QFont boldFont = option.font;
            boldFont.setBold(true);
            width += QFontMetrics(boldFont).horizontalAdvance(userName);
        } else {
            width += option.fontMetrics.horizontalAdvance(u"(%1)"_s.arg(userName)) + margin;
        }
    }
    return {width, height};
}

#include "moc_showbannedusersdelegate.cpp"
