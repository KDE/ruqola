/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "abstractusercheck.h"

class MarkAsFavoriteRoomCheck : public AbstractUserCheck
{
    Q_OBJECT
public:
    using AbstractUserCheck::AbstractUserCheck;

    [[nodiscard]] QString name() const override;

protected:
    void doStart() override;
    void checkState() override;
};
