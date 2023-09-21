/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDebug>
#include <QWidget>
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorRoomsEditBaseWidget : public QWidget
{
    Q_OBJECT
public:
    struct RoomEditInfo {
        QString name;
        QString topic;
        QString announcement;
        QString description;
        QString userOwnerName;
        bool featured = false;
        bool defaultRoom = false;
        bool favorite = false;
        bool readOnly = false;
        bool privateRoom = false;
        bool archived = false;
    };

    explicit AdministratorRoomsEditBaseWidget(QWidget *parent = nullptr);
    ~AdministratorRoomsEditBaseWidget() override;

    [[nodiscard]] virtual RoomEditInfo roomEditInfo() const = 0;
    virtual void setRoomEditInfo(const RoomEditInfo &newRoomEditInfo) = 0;
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AdministratorRoomsEditBaseWidget::RoomEditInfo &t);
