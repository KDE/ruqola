/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsearchroomforteamwidget.h"
#include "common/flowlayout.h"
#include "misc/clickablewidget.h"
#include <QVBoxLayout>

TeamSearchRoomForTeamWidget::TeamSearchRoomForTeamWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchRoomLineEdit(new AddTeamRoomCompletionLineEdit(account, this))
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
    addUsers.reserve(mMap.count());
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
    addUsers.reserve(mMap.count());
    while (i.hasNext()) {
        i.next();
        addUsers << i.value()->name();
    }
    return addUsers;
}
