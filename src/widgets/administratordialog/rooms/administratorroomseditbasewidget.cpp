/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

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
    d << "Name " << t.name;
    d << "featured " << t.featured;
    d << "defaultRoom " << t.defaultRoom;
    d << "comment " << t.topic;
    d << "announcement " << t.announcement;
    d << "description " << t.description;
    d << "readOnly " << t.readOnly;
    d << "privateRoom " << t.privateRoom;
    d << "archived " << t.archived;
    d << "userOwnerName " << t.userOwnerName;
    return d;
}
