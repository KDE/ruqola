/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_export.h"
#include <QStringList>

class LIBRUQOLAWIDGETS_EXPORT WhatsNewInfo
{
public:
    WhatsNewInfo();
    ~WhatsNewInfo();

    [[nodiscard]] QStringList changes() const;
    void setChanges(const QStringList &newChanges);

    [[nodiscard]] QStringList newFeatures() const;
    void setNewFeatures(const QStringList &newNewFeatures);

    [[nodiscard]] QStringList bugFixings() const;
    void setBugFixings(const QStringList &newBugFixings);

private:
    QStringList mChanges;
    QStringList mNewFeatures;
    QStringList mBugFixings;
};
