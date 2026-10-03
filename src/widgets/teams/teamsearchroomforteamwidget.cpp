/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "teamsearchroomforteamwidget.h"

#include "misc/clickablewidget.h"
#include <QVBoxLayout>
#include <TextAddonsWidgets/TextAddonsWidgetFlowLayout>

using namespace Qt::Literals::StringLiterals;
TeamSearchRoomForTeamWidget::TeamSearchRoomForTeamWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchRoomLineEdit(new AddTeamRoomCompletionLineEdit(account, this))
    , mFlowLayout(new TextAddonsWidgets::TextAddonsWidgetFlowLayout)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSearchRoomLineEdit->setObjectName(u"mSearchRoomLineEdit"_s);
    connect(mSearchRoomLineEdit, &AddTeamRoomCompletionLineEdit::newRoomName, this, &TeamSearchRoomForTeamWidget::slotAddNewName);
    mainLayout->addWidget(mSearchRoomLineEdit);

    mFlowLayout->setObjectName(u"mFlowLayout"_s);
    mainLayout->addLayout(mFlowLayout);
    mainLayout->addStretch();
}

TeamSearchRoomForTeamWidget::~TeamSearchRoomForTeamWidget()
{
    delete mFlowLayout;
}

void TeamSearchRoomForTeamWidget::slotAddNewName(const AddTeamRoomCompletionLineEdit::RoomCompletionInfo &info)
{
    const QByteArray &roomId = info.roomId;
    if (mMap.contains(roomId)) {
        return;
    }
    auto clickableWidget = new ClickableWidget(info.roomName, this);
    clickableWidget->setIdentifier(roomId);
    connect(clickableWidget, &ClickableWidget::removeClickableWidget, this, &TeamSearchRoomForTeamWidget::slotRemoveRoom);
    mFlowLayout->addWidget(clickableWidget);
    mMap.insert(roomId, clickableWidget);
    Q_EMIT roomListChanged(!mMap.isEmpty());
}

void TeamSearchRoomForTeamWidget::slotRemoveRoom(const QByteArray &roomId)
{
    ClickableWidget *userWidget = mMap.value(roomId);
    if (userWidget) {
        const int index = mFlowLayout->indexOf(userWidget);
        if (index != -1) {
            delete mFlowLayout->takeAt(index);
            mMap.remove(roomId);
            userWidget->deleteLater();
        }
    }
    Q_EMIT roomListChanged(!mMap.isEmpty());
}

QList<QByteArray> TeamSearchRoomForTeamWidget::roomIds() const
{
    QList<QByteArray> addRooms;
    addRooms.reserve(mMap.count());
    for (const auto &[key, value] : mMap.asKeyValueRange()) {
        addRooms << value->identifier();
    }
    return addRooms;
}

#include "moc_teamsearchroomforteamwidget.cpp"
