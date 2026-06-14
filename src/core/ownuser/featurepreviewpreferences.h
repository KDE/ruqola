/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QMap>
#include <QMetaType>
class QDebug;
class LIBRUQOLACORE_EXPORT FeaturePreviewPreferences
{
    Q_GADGET
public:
    enum FeaturePreviewType {
        None = 0,
        EnableTimestampMessageParser = 1,
        EnableDraftSupport = 2,
    };
    Q_ENUM(FeaturePreviewType)

    FeaturePreviewPreferences();
    ~FeaturePreviewPreferences();

    [[nodiscard]] bool operator==(const FeaturePreviewPreferences &other) const;
    void parseFeaturePreview(const QJsonArray &array);

    [[nodiscard]] QMap<FeaturePreviewType, bool> previewStatus() const;
    void setPreviewStatus(const QMap<FeaturePreviewType, bool> &newPreviewStatus);

    [[nodiscard]] bool hasFeaturePreview() const;

    [[nodiscard]] bool hasFeature(FeaturePreviewPreferences::FeaturePreviewType type) const;

    [[nodiscard]] bool serverHasPreviewFeature(FeaturePreviewPreferences::FeaturePreviewType type) const;

private:
    QMap<FeaturePreviewType, bool> mPreviewStatus;
};

LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const FeaturePreviewPreferences &t);
