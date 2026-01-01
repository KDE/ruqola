/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorroomseditbasewidget.h"

AdministratorRoomsEditBaseWidget::AdministratorRoomsEditBaseWidget(QWidget *parent)
    : QWidget{parent}
{
}

AdministratorRoomsEditBaseWidget::~AdministratorRoomsEditBaseWidget() = default;

QDebug operator<<(QDebug d, const AdministratorRoomsEditBaseWidget::RoomEditInfo &t)
{
    d.space() << "Name " << t.name;
    d.space() << "featured " << t.featured;
    d.space() << "defaultRoom " << t.defaultRoom;
    d.space() << "comment " << t.topic;
    d.space() << "announcement " << t.announcement;
    d.space() << "description " << t.description;
    d.space() << "readOnly " << t.readOnly;
    d.space() << "privateRoom " << t.privateRoom;
    d.space() << "archived " << t.archived;
    d.space() << "userOwnerName " << t.userOwnerName;
    d.space() << "favorite " << t.favorite;
    return d;
}

#include "moc_administratorroomseditbasewidget.cpp"
