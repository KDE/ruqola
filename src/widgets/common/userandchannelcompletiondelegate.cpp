/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userandchannelcompletiondelegate.h"

#include "common/delegatepaintutil.h"
#include "misc/avatarcachemanager.h"
#include "model/inputcompletermodel.h"

#include <KLocalizedString>

#include <QPainter>

using namespace Qt::Literals::StringLiterals;
UserAndChannelCompletionDelegate::UserAndChannelCompletionDelegate(QObject *parent)
    : QItemDelegate{parent}
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::UserAndRoom, this))
{
}

UserAndChannelCompletionDelegate::~UserAndChannelCompletionDelegate() = default;

void UserAndChannelCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] icon ? status name (username) (description if necessary)
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

    QFontMetrics fontMetrics(boldFont);
    const int iconSize = option.rect.height() - 4;
    const int iconY = option.rect.y() + 2;

    int xPos = 0;
    const Utils::AvatarInfo info = index.data(InputCompleterModel::AvatarInfo).value<Utils::AvatarInfo>();
    if (info.isValid()) {
        const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(option.widget, info, iconSize);
        if (!pix.isNull()) {
            painter->drawPixmap(margin, iconY, iconSize, iconSize, pix);
        }
        xPos = margin + iconSize;
    }

    const QIcon iconStatus = index.data(InputCompleterModel::IconStatus).value<QIcon>();
    if (!iconStatus.isNull()) {
        painter->drawPixmap(margin + xPos, iconY, iconSize, iconSize, iconStatus.pixmap(iconSize, iconSize));
        xPos += margin + iconSize;
    }

    const QString name = index.data(InputCompleterModel::DisplayName).toString();
    const QString userName = index.data(InputCompleterModel::UserName).toString();
    int nameWidth = -1;
    const int defaultCharHeight = option.rect.y() + (option.rect.height() - fontMetrics.height()) / 2 + fontMetrics.ascent();
    if (name.isEmpty()) {
        nameWidth = fontMetrics.horizontalAdvance(userName);
        painter->drawText(xPos + margin, defaultCharHeight, userName);
        xPos += nameWidth;
    } else {
        nameWidth = fontMetrics.horizontalAdvance(name);
        painter->drawText(xPos + margin, defaultCharHeight, name);
        xPos += nameWidth;
        if (!userName.isEmpty()) {
            painter->setFont(oldFont);
            fontMetrics = QFontMetrics(oldFont);
            nameWidth = fontMetrics.horizontalAdvance(userName);
            DelegatePaintUtil::drawLighterText(painter, u"(%1)"_s.arg(userName), QPoint(xPos + margin * 2, defaultCharHeight));
            xPos += nameWidth;
        }
    }

    const QString description = index.data(InputCompleterModel::Description).toString();
    if (!description.isEmpty()) {
        QFont italicFont = oldFont;
        italicFont.setItalic(true);
        painter->setFont(italicFont);

        painter->drawText(xPos + margin * 2, defaultCharHeight, description);
    }

    painter->setFont(oldFont);

    if (index.data(InputCompleterModel::OutsideRoom).toBool()) {
        fontMetrics = QFontMetrics(oldFont);
        const QString inRoomStr = i18n("Not in channel");
        const int inRoomStrWidth = fontMetrics.horizontalAdvance(inRoomStr);

        painter->drawText(option.rect.width() - inRoomStrWidth - margin, defaultCharHeight, inRoomStr);
    }
    painter->setPen(oldPen);
}

void UserAndChannelCompletionDelegate::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mAvatarCacheManager->setCurrentRocketChatAccount(newRocketChatAccount);
}

QSize UserAndChannelCompletionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QSize size = QItemDelegate::sizeHint(option, index);
    return size + QSize(0, 4);
}

#include "moc_userandchannelcompletiondelegate.cpp"
