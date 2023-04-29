/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>

class Room;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelRolesInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelRolesInfoWidget(QWidget *parent = nullptr);
    ~ChannelRolesInfoWidget() override;

    void setRoom(Room *room);
};
