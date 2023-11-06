/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "userandchannelcompletiondelegate.h"
#include "common/delegatepaintutil.h"
#include "model/inputcompletermodel.h"

#include <QPainter>

UserAndChannelCompletionDelegate::UserAndChannelCompletionDelegate(QObject *parent)
    : QItemDelegate{parent}
{
}

UserAndChannelCompletionDelegate::~UserAndChannelCompletionDelegate() = default;

void UserAndChannelCompletionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] icon ? status name (username) (description if necessary)
    drawBackground(painter, option, index);

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    const int margin = DelegatePaintUtil::margin();

    // TODO
}
