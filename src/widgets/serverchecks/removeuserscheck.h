/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractusercheck.h"

/** Removes (kicks) a user from the room and verifies the server accepts it. */
class LIBRUQOLAWIDGETS_TESTS_EXPORT RemoveUsersCheck : public AbstractUserCheck
{
    Q_OBJECT
public:
    using AbstractUserCheck::AbstractUserCheck;

    [[nodiscard]] QString name() const override;

protected:
    void doStart() override;
};
