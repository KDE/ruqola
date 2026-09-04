/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "showreadreceiptsdelegate.h"
#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/readreceiptsmodel.h"
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
    // [M] icon ? name (username)                    timestamp
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

    const QFontMetrics fontMetrics(boldFont);
    const int defaultCharHeight = option.rect.y() + (option.rect.height() - fontMetrics.height()) / 2 + fontMetrics.ascent();

    // The timestamp is right aligned and drawn with the view font, so paint it first:
    // it tells us how much room is left for the name/username.
    int rightLimit = option.rect.right() - margin;
    const qint64 timeStamp = index.data(ReadReceiptsModel::TimeStamp).toLongLong();
    if (timeStamp > 0) {
        const QString timeStampStr = QLocale().toString(QDateTime::fromMSecsSinceEpoch(timeStamp), QLocale::ShortFormat);
        const int timeStampWidth = option.fontMetrics.horizontalAdvance(timeStampStr);
        DelegatePaintUtil::drawLighterText(painter, timeStampStr, QPoint(rightLimit - timeStampWidth, defaultCharHeight));
        rightLimit -= timeStampWidth + margin;
    }

    int xPos = option.rect.x();
    const Utils::AvatarInfo info = index.data(ReadReceiptsModel::AvatarInfo).value<Utils::AvatarInfo>();
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

    const QString name = index.data(ReadReceiptsModel::Name).toString();
    const QString userName = index.data(ReadReceiptsModel::UserName).toString();
    painter->setFont(boldFont);
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

void ShowReadReceiptsDelegate::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(newRocketChatAccount);
}

QSize ShowReadReceiptsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QSize size = QItemDelegate::sizeHint(option, index);
    const int margin = DelegatePaintUtil::margin();
    const int height = size.height() + 4;
    // QItemDelegate only measured the display role (the username). Account for what
    // paint() draws around it: the avatar, the bold name and the timestamp.
    int width = size.width() + margin;
    if (index.data(ReadReceiptsModel::AvatarInfo).value<Utils::AvatarInfo>().isValid()) {
        width += margin + height - 4;
    }
    if (const QString name = index.data(ReadReceiptsModel::Name).toString(); !name.isEmpty()) {
        QFont boldFont = option.font;
        boldFont.setBold(true);
        width += QFontMetrics(boldFont).horizontalAdvance(name) + margin;
    }
    if (const qint64 timeStamp = index.data(ReadReceiptsModel::TimeStamp).toLongLong(); timeStamp > 0) {
        const QString timeStampStr = QLocale().toString(QDateTime::fromMSecsSinceEpoch(timeStamp), QLocale::ShortFormat);
        width += option.fontMetrics.horizontalAdvance(timeStampStr) + margin;
    }
    return {width, height};
}

#include "moc_showreadreceiptsdelegate.cpp"
