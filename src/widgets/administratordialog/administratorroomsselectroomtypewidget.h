/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#ifndef ADMINISTRATORROOMSSELECTROOMTYPEWIDGET_H
#define ADMINISTRATORROOMSSELECTROOMTYPEWIDGET_H

#include <QWidget>

#include "libruqolawidgets_private_export.h"
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorRoomsSelectRoomTypeWidget : public QWidget
{
    Q_OBJECT
public:
    enum class FilterRoom {
        None = 0,
        DirectRooms = 1,
        PublicRooms = 2,
        PrivateRooms = 4,
        DiscussionRooms = 8,
    };
    Q_FLAGS(FilterRoom FilterRooms)
    Q_DECLARE_FLAGS(FilterRooms, FilterRoom)
    explicit AdministratorRoomsSelectRoomTypeWidget(QWidget *parent = nullptr);
    ~AdministratorRoomsSelectRoomTypeWidget();

Q_SIGNALS:
    void filterChanged(AdministratorRoomsSelectRoomTypeWidget::FilterRooms filters);

private:
    void slotFilterChanged();
    QCheckBox *const mDirectRooms;
    QCheckBox *const mPublicRooms;
    QCheckBox *const mPrivateRooms;
    QCheckBox *const mDiscussionRooms;
    //Omnichannel ?? what is it?
};

#endif // ADMINISTRATORROOMSSELECTROOMTYPEWIDGET_H
