/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"

/** Changes the channel announcement and verifies the new announcement arrives on the room, then restores it. */
class LIBRUQOLAWIDGETS_TESTS_EXPORT AnnouncementChangeCheck : public AbstractServerCheck
{
    Q_OBJECT
public:
    using AbstractServerCheck::AbstractServerCheck;

    [[nodiscard]] QString name() const override;

protected:
    void doStart() override;
    void checkState() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void sendAnnouncement(const QString &topic);
    QString mOriginalAnnouncement;
    QString mNewAnnouncement;
};
