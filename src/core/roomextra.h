/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"

#include <QSharedData>

class LIBRUQOLACORE_EXPORT RoomExtra : public QSharedData
{
public:
    RoomExtra();
    ~RoomExtra();
};
