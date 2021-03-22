/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "channelinfowidget.h"
#include "channelinfoeditablewidget.h"
#include "channelinfoprunewidget.h"
#include "channelinforeadonlywidget.h"
#include "misc/systemmessagescombobox.h"
#include "permissionmanager.h"
#include "restapirequest.h"
#include "rocketchataccount.h"
#include "room.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <KPasswordLineEdit>
#include <KSeparator>
#include <QCheckBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTextDocument>
#include <QToolButton>

ChannelInfoWidget::ChannelInfoWidget(QWidget *parent)
    : QWidget(parent)
    , mStackedWidget(new QStackedWidget(this))
    , mChannelInfoEditableWidget(new ChannelInfoEditableWidget(this))
    , mChannelInfoReadOnlyWidget(new ChannelInfoReadOnlyWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    mStackedWidget->addWidget(mChannelInfoReadOnlyWidget);
    mStackedWidget->addWidget(mChannelInfoEditableWidget);
    connect(mChannelInfoEditableWidget, &ChannelInfoEditableWidget::channelDeleted, this, &ChannelInfoWidget::channelDeleted);
    connect(mChannelInfoEditableWidget, &ChannelInfoEditableWidget::fnameChanged, this, &ChannelInfoWidget::fnameChanged);
}

ChannelInfoWidget::~ChannelInfoWidget()
{
}

void ChannelInfoWidget::updateUiFromPermission()
{
    mChannelInfoEditableWidget->updateUiFromPermission();
}

RocketChatRestApi::SaveRoomSettingsJob::SaveRoomSettingsInfo ChannelInfoWidget::saveRoomSettingsInfo() const
{
    if (mRoom->canBeModify()) {
        return mChannelInfoEditableWidget->saveRoomSettingsInfo();
    }
    return {};
}

void ChannelInfoWidget::setRoom(Room *room)
{
    mRoom = room;
    if (mRoom->canBeModify()) {
        mStackedWidget->setCurrentWidget(mChannelInfoEditableWidget);
        mChannelInfoEditableWidget->updateEditableChannelInfo();
        mChannelInfoEditableWidget->updateRetentionValue();
        mChannelInfoEditableWidget->connectEditableWidget();
    } else {
        mChannelInfoReadOnlyWidget->setRoom(mRoom);
        mStackedWidget->setCurrentWidget(mChannelInfoReadOnlyWidget);
    }
}


