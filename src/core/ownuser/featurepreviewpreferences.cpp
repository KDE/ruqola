/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "featurepreviewpreferences.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
FeaturePreviewPreferences::FeaturePreviewPreferences() = default;

FeaturePreviewPreferences::~FeaturePreviewPreferences() = default;

bool FeaturePreviewPreferences::operator==(const FeaturePreviewPreferences &other) const
{
    return mPreviewStatus == other.mPreviewStatus;
}

QDebug operator<<(QDebug d, const FeaturePreviewPreferences &t)
{
    d.space() << "previewStatus:" << t.previewStatus();
    return d;
}

void FeaturePreviewPreferences::parseFeaturePreview(const QJsonArray &array)
{
    qDebug() << " FeaturePreviewPreferences::parseFeaturePreview(const QJsonArray &array) " << array;
    mPreviewStatus.clear();
    for (const auto &v : array) {
        const QJsonObject o = v.toObject();
        const QString name = o["name"_L1].toString();
        const bool value = o["value"_L1].toBool();
        if (name == "enable-timestamp-message-parser"_L1) {
            mPreviewStatus[EnableTimestampMessageParser] = value;
        } else if (name == "sidebarDrafts"_L1) {
            mPreviewStatus[EnableDraftSupport] = value;
        }
    }
    qDebug() << " CXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << previewStatus();
}

QMap<FeaturePreviewPreferences::FeaturePreviewType, bool> FeaturePreviewPreferences::previewStatus() const
{
    return mPreviewStatus;
}

void FeaturePreviewPreferences::setPreviewStatus(const QMap<FeaturePreviewType, bool> &newPreviewStatus)
{
    mPreviewStatus = newPreviewStatus;
}

bool FeaturePreviewPreferences::hasFeaturePreview() const
{
    return !mPreviewStatus.isEmpty();
}

bool FeaturePreviewPreferences::hasFeature(FeaturePreviewPreferences::FeaturePreviewType type) const
{
    return mPreviewStatus.value(type, false);
}

#include "moc_featurepreviewpreferences.cpp"
