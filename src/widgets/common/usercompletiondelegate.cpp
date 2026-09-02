/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usercompletiondelegate.h"
using namespace Qt::Literals::StringLiterals;

#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/usercompletermodel.h"

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

    if (!option.showDecorationSelected && (option.state & QStyle::State_Selected)) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    const int margin = DelegatePaintUtil::margin();
    const QFont oldFont = painter->font();

    QFont boldFont = oldFont;
    boldFont.setBold(true);
    painter->setFont(boldFont);

    QFontMetrics fontMetrics(boldFont);
    const int iconSize = option.rect.height() - 4;
    const int iconY = option.rect.y() + 2;

    int xPos = 0;
    const Utils::AvatarInfo info = index.data(UserCompleterModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(option.widget, info, iconSize);
        if (!pix.isNull()) {
            painter->drawPixmap(margin, iconY, iconSize, iconSize, pix);
        }
        xPos = margin + iconSize;
    }

    const QIcon iconStatus = index.data(UserCompleterModel::UserIconStatus).value<QIcon>();
    if (!iconStatus.isNull()) {
        painter->drawPixmap(margin + xPos, iconY, iconSize, iconSize, iconStatus.pixmap(iconSize, iconSize));
        xPos += margin + iconSize;
    }

    const QString name = index.data(UserCompleterModel::DisplayName).toString();
    const QString userName = index.data(UserCompleterModel::UserName).toString();
    const int defaultCharHeight = option.rect.y() + (option.rect.height() - fontMetrics.height()) / 2 + fontMetrics.ascent();
    if (name.isEmpty()) {
        painter->drawText(xPos + margin, defaultCharHeight, userName);
    } else {
        int nameWidth = fontMetrics.horizontalAdvance(name);
        painter->drawText(xPos + margin, defaultCharHeight, name);
        xPos += nameWidth;
        if (!userName.isEmpty()) {
            painter->setFont(oldFont);
            DelegatePaintUtil::drawLighterText(painter, u"(%1)"_s.arg(userName), QPoint(xPos + margin * 2, defaultCharHeight));
        }
    }
    painter->setFont(oldFont);
}

void UserCompletionDelegate::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(newRocketChatAccount);
}

QSize UserCompletionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QSize size = QItemDelegate::sizeHint(option, index);
    return size + QSize(0, 4);
}

#include "moc_usercompletiondelegate.cpp"
