/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usercompletiondelegate.h"
#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/usercompletermodel.h"

#include <QFontMetricsF>
#include <QPainter>

UserCompletionDelegate::UserCompletionDelegate(QObject *parent)
    : QItemDelegate{parent}
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::User, this))
{
}

UserCompletionDelegate::~UserCompletionDelegate() = default;

void UserCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] icon ? status name (username)
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
    const Utils::AvatarInfo info = index.data(UserCompleterModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const QRect displayRect(margin, option.rect.y(), option.rect.height(), option.rect.height());
        const QPixmap pix = mAvatarCacheManager->makeAvatarUrlPixmap(option.widget, info, option.rect.height());
        if (!pix.isNull()) {
            drawDecoration(painter, option, displayRect, pix);
            xPos = margin + option.rect.height();
        }
    }

    const QIcon iconStatus = index.data(UserCompleterModel::UserIconStatus).value<QIcon>();
    if (!iconStatus.isNull()) {
        const QRect displayRect(margin + xPos, option.rect.y(), option.rect.height(), option.rect.height());
        drawDecoration(painter, option, displayRect, iconStatus.pixmap(option.rect.height(), option.rect.height()));
        xPos += margin + option.rect.height();
    }

    QFontMetrics fontMetrics(boldFont);
    const QString name = index.data(UserCompleterModel::DisplayName).toString();
    const QString userName = index.data(UserCompleterModel::UserName).toString();
    int nameWidth = -1;
    const int defaultCharHeight = option.rect.y() + fontMetrics.ascent();
    if (name.isEmpty()) {
        // nameWidth = fontMetrics.horizontalAdvance(userName);
        painter->drawText(xPos + margin, defaultCharHeight, userName);
    } else {
        nameWidth = fontMetrics.horizontalAdvance(name);
        painter->drawText(xPos + margin, defaultCharHeight, name);
        xPos += nameWidth;
        if (!userName.isEmpty()) {
            painter->setFont(oldFont);
            // fontMetrics = QFontMetrics(oldFont);
            // nameWidth = fontMetrics.horizontalAdvance(userName);
            painter->drawText(xPos + margin * 2, defaultCharHeight, userName);
        }
    }
    painter->setFont(oldFont);
}

void UserCompletionDelegate::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(newRocketChatAccount);
}
