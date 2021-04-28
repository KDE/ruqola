/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "teamsearchroomforteamwidget.h"
#include "common/flowlayout.h"
#include "misc/clickablewidget.h"
#include <QVBoxLayout>

TeamSearchRoomForTeamWidget::TeamSearchRoomForTeamWidget(QWidget *parent)
    : QWidget(parent)
    , mSearchRoomLineEdit(new AddTeamRoomCompletionLineEdit(this))
    , mFlowLayout(new FlowLayout)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchRoomLineEdit->setObjectName(QStringLiteral("mSearchRoomLineEdit"));
    connect(mSearchRoomLineEdit, &AddTeamRoomCompletionLineEdit::newRoomName, this, &TeamSearchRoomForTeamWidget::slotAddNewName);
    mainLayout->addWidget(mSearchRoomLineEdit);

    mFlowLayout->setObjectName(QStringLiteral("mFlowLayout"));
    mainLayout->addLayout(mFlowLayout);
    mainLayout->addStretch();
}

TeamSearchRoomForTeamWidget::~TeamSearchRoomForTeamWidget()
{
    delete mFlowLayout;
}

void TeamSearchRoomForTeamWidget::slotAddNewName(const AddTeamRoomCompletionLineEdit::RoomCompletionInfo &info)
{
    const QString &roomName = info.roomName;
    if (mMap.contains(roomName)) {
        return;
    }
    auto clickableWidget = new ClickableWidget(roomName, this);
    clickableWidget->setIdentifier(info.roomId);
    connect(clickableWidget, &ClickableWidget::removeClickableWidget, this, &TeamSearchRoomForTeamWidget::slotRemoveRoom);
    mFlowLayout->addWidget(clickableWidget);
    mMap.insert(roomName, clickableWidget);
    Q_EMIT userListChanged(!mMap.isEmpty());
}

void TeamSearchRoomForTeamWidget::slotRemoveRoom(const QString &name)
{
    ClickableWidget *userWidget = mMap.value(name);
    if (userWidget) {
        const int index = mFlowLayout->indexOf(userWidget);
        if (index != -1) {
            delete mFlowLayout->takeAt(index);
            mMap.remove(name);
            delete userWidget;
        }
    }
    Q_EMIT userListChanged(!mMap.isEmpty());
}

void TeamSearchRoomForTeamWidget::setPlaceholderText(const QString &str)
{
    mSearchRoomLineEdit->setPlaceholderText(str);
}

QString TeamSearchRoomForTeamWidget::placeHolderText() const
{
    return mSearchRoomLineEdit->placeholderText();
}

QStringList TeamSearchRoomForTeamWidget::roomIds() const
{
    QStringList addUsers;
    QMapIterator<QString, ClickableWidget *> i(mMap);
    while (i.hasNext()) {
        i.next();
        addUsers << i.value()->identifier();
    }
    return addUsers;
}

QStringList TeamSearchRoomForTeamWidget::names() const
{
    QStringList addUsers;
    QMapIterator<QString, ClickableWidget *> i(mMap);
    while (i.hasNext()) {
        i.next();
        addUsers << i.value()->name();
    }
    return addUsers;
}
