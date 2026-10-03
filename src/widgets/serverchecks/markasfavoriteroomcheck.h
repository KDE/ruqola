/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "abstractservercheck.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT MarkAsFavoriteRoomCheck : public AbstractServerCheck
{
    Q_OBJECT
public:
    using AbstractServerCheck::AbstractServerCheck;

    [[nodiscard]] QString name() const override;

protected:
    void doStart() override;
    void checkState() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void changeFavorite(bool state);
    bool mOriginalFavoriteState = false;
};
