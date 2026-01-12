/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"

class QDebug;
class LIBRUQOLACORE_EXPORT FeaturePreviewPreferences
{
public:
    FeaturePreviewPreferences();
    ~FeaturePreviewPreferences();

    [[nodiscard]] bool operator==(const FeaturePreviewPreferences &other) const;
    void parseFeaturePreview(const QJsonArray &array);
};

LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const FeaturePreviewPreferences &t);
