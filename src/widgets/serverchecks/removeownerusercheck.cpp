/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "removeownerusercheck.h"

#include <KLocalizedString>

QString RemoveOwnerUserCheck::name() const
{
    return i18n("Remove Owner User");
}

void RemoveOwnerUserCheck::doStart()
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
                    [this, user, userId](bool, const QString &) {
                        changeChannelOwnerUser(
                            userId,
                            [this, user, userId](bool ok, const QString &error) {
                                if (ok) {
                                    reportPassed();
                                } else {
                                    reportFailed(i18n("Could not remove owner \"%1\": %2", user, error));
                                }
                            },
                            false);
                    },
                    true);
            });
        });
    });
}

#include "moc_removeownerusercheck.cpp"
