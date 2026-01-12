/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "featurepreviewpreferences.h"
#include <QDebug>
#include <QJsonArray>

FeaturePreviewPreferences::FeaturePreviewPreferences() = default;

FeaturePreviewPreferences::~FeaturePreviewPreferences() = default;

bool FeaturePreviewPreferences::operator==(const FeaturePreviewPreferences &other) const
{
    // TODO
    return true;
}

QDebug operator<<(QDebug d, const FeaturePreviewPreferences &t)
{
    // TODO
    return d;
}

void FeaturePreviewPreferences::parseFeaturePreview(const QJsonArray &array)
{
    // TODO
}
