/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"
#include <functional>

namespace RocketChatRestApi
{
class RestApiAbstractJob;
}

/**
 * Base class for the "user management" checks (add/remove/ban/unban). These drive
 * REST jobs and consider the server behaving correctly when the job reports done.
 *
 * The target user is picked so we never act on ourselves: "dfaure" when the current
 * user is "laurent", "laurent" otherwise.
 */
class LIBRUQOLAWIDGETS_TESTS_EXPORT AbstractUserCheck : public AbstractServerCheck
{
    Q_OBJECT
public:
    using AbstractServerCheck::AbstractServerCheck;

protected:
    // These checks are REST-driven, not message-model driven.
    void checkState() override
    {
    }

    using JobCallback = std::function<void(bool ok, const QString &error)>;

    [[nodiscard]] QString targetUserName() const;

    void resolveUserId(const QString &userName, const std::function<void(bool ok, const QByteArray &userId)> &callback);
    void invite(const QByteArray &userId, const JobCallback &callback);
    void kick(const QByteArray &userId, const JobCallback &callback);
    void changeChannelOwnerUser(const QByteArray &userId, const JobCallback &callback, bool add);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void connectFailure(RocketChatRestApi::RestApiAbstractJob *job, const JobCallback &callback);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool isPrivateRoom() const;
};
