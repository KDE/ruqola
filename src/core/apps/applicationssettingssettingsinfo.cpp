/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssettingsinfo.h"
#include "ruqola_debug.h"
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsSettingsInfo::ApplicationsSettingsSettingsInfo() = default;

ApplicationsSettingsSettingsInfo::~ApplicationsSettingsSettingsInfo() = default;

void ApplicationsSettingsSettingsInfo::parseSettings(const QJsonObject &obj)
{
    mI18nDescription = obj["i18nDescription"_L1].toString();
    mI18nLabel = obj["i18nLabel"_L1].toString();
    mId = obj["id"_L1].toString();
    mSettingType = convertStringToType(obj["type"_L1].toString());
    if (mSettingType == ApplicationsSettingsSettingsInfo::SettingType::String) {
        mPackageValue = obj["packageValue"_L1].toString();
    } else if (mSettingType == ApplicationsSettingsSettingsInfo::SettingType::Boolean) {
        mPackageValue = obj["packageValue"_L1].toBool() ? QStringLiteral("true") : QStringLiteral("false");
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown type " << obj["packageValue"_L1];
    }

    // TODO add createdAt/public/required/updatedAt
}

bool ApplicationsSettingsSettingsInfo::operator==(const ApplicationsSettingsSettingsInfo &other) const
{
    return mI18nDescription == other.mI18nDescription && mI18nLabel == other.mI18nLabel && mId == other.mId && mPackageValue == other.mPackageValue
        && mSettingType == other.mSettingType;
}

ApplicationsSettingsSettingsInfo::SettingType ApplicationsSettingsSettingsInfo::convertStringToType(const QString &str)
{
    if (str == "string"_L1) {
        return SettingType::String;
    } else if (str == "boolean"_L1) {
        return SettingType::Boolean;
    }
    qCWarning(RUQOLA_LOG) << "Unknown type " << str;
    return SettingType::Unknown;
}

QString ApplicationsSettingsSettingsInfo::i18nDescription() const
{
    return mI18nDescription;
}

void ApplicationsSettingsSettingsInfo::setI18nDescription(const QString &newI18nDescription)
{
    mI18nDescription = newI18nDescription;
}

QString ApplicationsSettingsSettingsInfo::i18nLabel() const
{
    return mI18nLabel;
}

void ApplicationsSettingsSettingsInfo::setI18nLabel(const QString &newI18nLabel)
{
    mI18nLabel = newI18nLabel;
}

QString ApplicationsSettingsSettingsInfo::id() const
{
    return mId;
}

void ApplicationsSettingsSettingsInfo::setId(const QString &newId)
{
    mId = newId;
}

QString ApplicationsSettingsSettingsInfo::packageValue() const
{
    return mPackageValue;
}

void ApplicationsSettingsSettingsInfo::setPackageValue(const QString &newPackageValue)
{
    mPackageValue = newPackageValue;
}

ApplicationsSettingsSettingsInfo::SettingType ApplicationsSettingsSettingsInfo::settingType() const
{
    return mSettingType;
}

void ApplicationsSettingsSettingsInfo::setSettingType(SettingType newSettingType)
{
    mSettingType = newSettingType;
}

QDebug operator<<(QDebug d, const ApplicationsSettingsSettingsInfo &t)
{
    d.space() << "i18nDescription" << t.i18nDescription();
    d.space() << "i18nLabel" << t.i18nLabel();
    d.space() << "id" << t.id();
    d.space() << "packageValue" << t.packageValue();
    d.space() << "settingType" << t.settingType();
    return d;
}

#include "moc_applicationssettingssettingsinfo.cpp"
