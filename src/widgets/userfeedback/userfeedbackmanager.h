/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

namespace KUserFeedback
{
class Provider;
}
class UserFeedBackManager
{
public:
    ~UserFeedBackManager();
    static UserFeedBackManager *self();

    [[nodiscard]] KUserFeedback::Provider *userFeedbackProvider() const;

private:
    UserFeedBackManager();
    KUserFeedback::Provider *const mUserFeedbackProvider;
};
