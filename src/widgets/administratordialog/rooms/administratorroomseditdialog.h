/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "administratorroomseditbasewidget.h"
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorRoomsEditDialog : public QDialog
{
    Q_OBJECT
public:
    enum RoomType {
        Unknown,
        DirectRoom,
        Team,
        Group,
        Channel,
    };

    explicit AdministratorRoomsEditDialog(RoomType roomType, QWidget *parent = nullptr);
    ~AdministratorRoomsEditDialog() override;

    [[nodiscard]] RoomType roomType() const;

    [[nodiscard]] AdministratorRoomsEditBaseWidget::RoomEditInfo roomEditInfo() const;
    void setRoomEditInfo(const AdministratorRoomsEditBaseWidget::RoomEditInfo &newRoomEditInfo);

private:
    AdministratorRoomsEditBaseWidget *mAdministratorRoomEditWidget = nullptr;
    const RoomType mRoomType;
};
