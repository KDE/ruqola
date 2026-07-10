/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addmoderatorusercheck.h"

#include <KLocalizedString>

QString AddModeratorUserCheck::name() const
{
    return i18n("Add Moderator User");
}

void AddModeratorUserCheck::doStart()
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
                changeChannelModeratorUser(
                    userId,
                    [this, user](bool ok, const QString &error) {
                        if (ok) {
                            reportPassed();
                        } else {
                            reportFailed(i18n("Could not add moderator \"%1\": %2", user, error));
                        }
                    },
                    true);
            });
        });
    });
}

#include "moc_addmoderatorusercheck.cpp"
