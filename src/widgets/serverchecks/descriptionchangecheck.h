/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "abstractservercheck.h"

/** Changes the channel description and verifies the new description arrives on the room, then restores it. */
class LIBRUQOLAWIDGETS_TESTS_EXPORT DescriptionChangeCheck : public AbstractServerCheck
{
    Q_OBJECT
public:
    using AbstractServerCheck::AbstractServerCheck;

    [[nodiscard]] QString name() const override;

protected:
    void doStart() override;
    void checkState() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void sendDescription(const QString &topic);
    QString mOriginalDescription;
    QString mNewDescription;
};
