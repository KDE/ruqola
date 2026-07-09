/*
   SPDX-FileCopyrightText: 2026 David Faure <david.faure@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addownerusercheck.h"

#include <KLocalizedString>

QString AddOwnerUserCheck::name() const
{
    return i18n("Change Owner User");
}

void AddOwnerUserCheck::doStart()
{
    if (!mAccount) {
        reportFailed(i18n("No account."));
        return;
    }
    const QString user = targetUserName();
    // Ensure the user is absent first, so re-running the check stays reliable.
    resolveUserId(user, [this, user](bool resolved, const QByteArray &userId) {
        if (!resolved) {
            reportFailed(i18n("Could not resolve user \"%1\".", user));
            return;
        }
        kick(userId, [this, user, userId](bool, const QString &) {
            invite(userId, [this, user, userId](bool, const QString &) {
                changeChannelOwnerUser(
                    userId,
                    [this, user](bool ok, const QString &error) {
                        if (ok) {
                            reportPassed();
                        } else {
                            reportFailed(i18n("Could not add owner \"%1\": %2", user, error));
                        }
                    },
                    true);
            });
        });
    });
}

#include "moc_addownerusercheck.cpp"
