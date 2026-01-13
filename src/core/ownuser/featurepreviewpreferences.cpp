/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "featurepreviewpreferences.h"
#include <QDebug>
#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;
FeaturePreviewPreferences::FeaturePreviewPreferences() = default;

FeaturePreviewPreferences::~FeaturePreviewPreferences() = default;

bool FeaturePreviewPreferences::operator==(const FeaturePreviewPreferences &other) const
{
    return mFeaturePreviewTypes == other.featurePreviewTypes();
}

QDebug operator<<(QDebug d, const FeaturePreviewPreferences &t)
{
    d.space() << "FeaturePreviewType:" << t.featurePreviewTypes();
    return d;
}

void FeaturePreviewPreferences::parseFeaturePreview(const QJsonArray &array)
{
    // TODO clear before ?
    for (const auto &v : array) {
        const QJsonObject o = v.toObject();
        const QString name = o["name"_L1].toString();
        const bool value = (o["value"_L1] == "true"_L1);
        if (name == "enable-timestamp-message-parser"_L1) {
            assignSettingValue(value, EnableTimestampMessageParser);
        }
    }
}

void FeaturePreviewPreferences::assignSettingValue(bool value, FeaturePreviewType type)
{
    if (value) {
        mFeaturePreviewTypes |= type;
    } else {
        mFeaturePreviewTypes &= ~type;
    }
}

bool FeaturePreviewPreferences::hasFeature(FeaturePreviewType type) const
{
    return mFeaturePreviewTypes & type;
}

FeaturePreviewPreferences::FeaturePreviewTypes FeaturePreviewPreferences::featurePreviewTypes() const
{
    return mFeaturePreviewTypes;
}

void FeaturePreviewPreferences::setFeaturePreviewTypes(const FeaturePreviewTypes &newFeaturePreviewTypes)
{
    mFeaturePreviewTypes = newFeaturePreviewTypes;
}

bool FeaturePreviewPreferences::hasFeaturePreview() const
{
    return mFeaturePreviewTypes != FeaturePreviewType::None;
}

#include "moc_featurepreviewpreferences.cpp"
