/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showreadreceiptsdelegate.h"
#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/bannedusersmodel.h"
#include "utils.h"
#include <QPainter>

using namespace Qt::Literals::StringLiterals;
ShowReadReceiptsDelegate::ShowReadReceiptsDelegate(QObject *parent)
    : QItemDelegate{parent}
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::User, this))
{
}

ShowReadReceiptsDelegate::~ShowReadReceiptsDelegate() = default;

void ShowReadReceiptsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] icon ? name (username)
    drawBackground(painter, option, index);

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    const int margin = DelegatePaintUtil::margin();
    const QFont oldFont = painter->font();

    QFont boldFont = oldFont;
    boldFont.setBold(true);
    painter->setFont(boldFont);

    int xPos = -1;
    const Utils::AvatarInfo info = index.data(BannedUsersModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const QRect displayRect(margin, option.rect.y(), option.rect.height(), option.rect.height());
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(option.widget, info, option.rect.height());
        if (!pix.isNull()) {
            drawDecoration(painter, option, displayRect, pix);
        }
        // Add extra size even if we don't have avatar pix
        xPos = margin + option.rect.height();
    }

    const QFontMetrics fontMetrics(boldFont);
    const QString name = index.data(BannedUsersModel::Name).toString();
    const QString userName = index.data(BannedUsersModel::UserName).toString();
    const int defaultCharHeight = option.rect.y() + fontMetrics.ascent();
    if (name.isEmpty()) {
        painter->drawText(xPos + margin, defaultCharHeight, userName);
    } else {
        const int nameWidth = fontMetrics.horizontalAdvance(name);
        painter->drawText(xPos + margin, defaultCharHeight, name);
        xPos += nameWidth;
        if (!userName.isEmpty()) {
            painter->setFont(oldFont);
            DelegatePaintUtil::drawLighterText(painter, u"(%1)"_s.arg(userName), QPoint(xPos + margin * 2, defaultCharHeight));
        }
    }
    painter->setFont(oldFont);
}

void ShowReadReceiptsDelegate::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(newRocketChatAccount);
}

QSize ShowReadReceiptsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QSize size = QItemDelegate::sizeHint(option, index);
    return size + QSize(0, 4 * option.widget->devicePixelRatioF());
}

#include "moc_showreadreceiptsdelegate.cpp"
