/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssettingsinfo.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(ApplicationsSettingsSettingsInfo, Ruqola_ApplicationsSettingsSettingsInfo)

#include "ruqola_debug.h"
#include <QJsonArray>
using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsSettingsInfo::ApplicationsSettingsSettingsInfo() = default;

ApplicationsSettingsSettingsInfo::~ApplicationsSettingsSettingsInfo() = default;

void ApplicationsSettingsSettingsInfo::parseSettings(const QJsonObject &obj)
{
    mJsonObj = obj;
    mI18nDescription = obj["i18nDescription"_L1].toString();
    mI18nLabel = obj["i18nLabel"_L1].toString();
    mId = obj["id"_L1].toString();
    mRequired = obj["required"_L1].toBool(false);
    mMultiLine = obj["multiline"_L1].toBool(false);
    mSettingType = convertStringToType(obj["type"_L1].toString());
    if (mSettingType == ApplicationsSettingsSettingsInfo::SettingType::String) {
        mPackageValue = obj["packageValue"_L1].toString();
        if (obj.contains("value"_L1)) {
            mValue = obj["value"_L1].toString();
        }
    } else if (mSettingType == ApplicationsSettingsSettingsInfo::SettingType::Boolean) {
        mPackageValue = obj["packageValue"_L1].toBool();
        if (obj.contains("value"_L1)) {
            mValue = obj["value"_L1].toBool();
        }
    } else if (mSettingType == ApplicationsSettingsSettingsInfo::SettingType::Select) {
        mPackageValue = obj["packageValue"_L1].toString();
        if (obj.contains("value"_L1)) {
            mValue = obj["value"_L1].toString();
        }
        // Load allow selected values
        const QJsonArray array = obj["values"_L1].toArray();
        for (const auto &r : array) {
            mValues.insert(r["key"_L1].toString(), r["i18nLabel"_L1].toString());
        }
    } else if (mSettingType == ApplicationsSettingsSettingsInfo::SettingType::Int) {
        mPackageValue = obj["packageValue"_L1].toInt();
        if (obj.contains("value"_L1)) {
            mValue = obj["value"_L1].toInt();
        }
    } else {
        qCWarning(RUQOLA_LOG) << "Unknown type " << obj["packageValue"_L1];
    }

    // TODO add createdAt/public/updatedAt
}

bool ApplicationsSettingsSettingsInfo::operator==(const ApplicationsSettingsSettingsInfo &other) const
{
    return mI18nDescription == other.mI18nDescription && mI18nLabel == other.mI18nLabel && mId == other.mId && mPackageValue == other.mPackageValue
        && mSettingType == other.mSettingType && mValues == other.mValues && mRequired == other.mRequired && other.mValue == mValue
        && other.mMultiLine == mMultiLine;
}

ApplicationsSettingsSettingsInfo::SettingType ApplicationsSettingsSettingsInfo::convertStringToType(const QString &str)
{
    if (str == "string"_L1) {
        return SettingType::String;
    } else if (str == "boolean"_L1) {
        return SettingType::Boolean;
    } else if (str == "select"_L1) {
        return SettingType::Select;
    } else if (str == "int"_L1) {
        return SettingType::Int;
    }
    qCWarning(RUQOLA_LOG) << "Unknown type " << str;
    return SettingType::Unknown;
}

QJsonObject ApplicationsSettingsSettingsInfo::jsonObj() const
{
    return mJsonObj;
}

bool ApplicationsSettingsSettingsInfo::multiLine() const
{
    return mMultiLine;
}

void ApplicationsSettingsSettingsInfo::setMultiLine(bool newMultiLine)
{
    mMultiLine = newMultiLine;
}

QVariant ApplicationsSettingsSettingsInfo::value() const
{
    return mValue;
}

void ApplicationsSettingsSettingsInfo::setValue(const QVariant &newValue)
{
    mValue = newValue;
}

bool ApplicationsSettingsSettingsInfo::required() const
{
    return mRequired;
}

void ApplicationsSettingsSettingsInfo::setRequired(bool newRequired)
{
    mRequired = newRequired;
}

QMap<QString, QString> ApplicationsSettingsSettingsInfo::values() const
{
    return mValues;
}

void ApplicationsSettingsSettingsInfo::setValues(const QMap<QString, QString> &newValues)
{
    mValues = newValues;
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

QVariant ApplicationsSettingsSettingsInfo::packageValue() const
{
    return mPackageValue;
}

void ApplicationsSettingsSettingsInfo::setPackageValue(const QVariant &newPackageValue)
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
    d.space() << "value" << t.value();
    d.space() << "settingType" << t.settingType();
    d.space() << "values" << t.values();
    d.space() << "required" << t.required();
    d.space() << "multiLine" << t.multiLine();

    return d;
}

#include "moc_applicationssettingssettingsinfo.cpp"
