/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"

class QDebug;
class LIBRUQOLACORE_EXPORT FeaturePreviewPreferences
{
    Q_GADGET
public:
    enum FeaturePreviewType {
        None = 0,
        EnableTimestampMessageParser = 1,
    };
    Q_DECLARE_FLAGS(FeaturePreviewTypes, FeaturePreviewType)
    Q_FLAG(FeaturePreviewTypes)

    FeaturePreviewPreferences();
    ~FeaturePreviewPreferences();

    [[nodiscard]] bool operator==(const FeaturePreviewPreferences &other) const;
    void parseFeaturePreview(const QJsonArray &array);

    [[nodiscard]] FeaturePreviewTypes featurePreviewTypes() const;
    void setFeaturePreviewTypes(const FeaturePreviewTypes &newFeaturePreviewTypes);

    [[nodiscard]] bool hasFeature(FeaturePreviewType type) const;

private:
    LIBRUQOLACORE_NO_EXPORT void assignSettingValue(bool value, FeaturePreviewType type);
    FeaturePreviewTypes mFeaturePreviewTypes = FeaturePreviewType::None;
};

LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const FeaturePreviewPreferences &t);
