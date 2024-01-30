/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "model/adminroomsfilterproxymodel.h"

#include "libruqolawidgets_private_export.h"
class QCheckBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorRoomsSelectRoomTypeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdministratorRoomsSelectRoomTypeWidget(QWidget *parent = nullptr);
    ~AdministratorRoomsSelectRoomTypeWidget() override;

Q_SIGNALS:
    void filterChanged(AdminRoomsFilterProxyModel::FilterRooms filters);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotFilterChanged();
    QCheckBox *const mDirectRooms;
    QCheckBox *const mPublicRooms;
    QCheckBox *const mPrivateRooms;
    QCheckBox *const mDiscussionRooms;
    QCheckBox *const mTeamRooms;
    QCheckBox *const mOmniChannel;
};
