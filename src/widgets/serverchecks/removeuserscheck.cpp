/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeuserscheck.h"

#include <KLocalizedString>

QString RemoveUsersCheck::name() const
{
    return i18n("Remove user from room");
}

void RemoveUsersCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    const QString user = targetUserName();
    resolveUserId(user, [this, user](bool resolved, const QByteArray &userId) {
        if (!resolved) {
            reportFailed(i18n("Could not resolve user \"%1\".", user));
            return;
        }
        // Ensure the user is present first, so there is something to remove.
        invite(userId, [this, user, userId](bool, const QString &) {
            kick(userId, [this, user](bool ok, const QString &error) {
                if (ok) {
                    reportPassed();
                } else {
                    reportFailed(i18n("Could not remove \"%1\": %2", user, error));
                }
            });
        });
    });
}

#include "moc_removeuserscheck.cpp"
