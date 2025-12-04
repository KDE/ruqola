/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqolacore_export.h"
#include <QJsonObject>
class LIBRUQOLACORE_EXPORT ApplicationsSettingsSettingsInfo
{
    Q_GADGET
public:
    enum class SettingType : uint8_t {
        Unknown,
        String,
        Boolean,
        Select,
        Int,
    };
    Q_ENUM(SettingType);

    ApplicationsSettingsSettingsInfo();
    ~ApplicationsSettingsSettingsInfo();

    void parseSettings(const QJsonObject &obj);
    [[nodiscard]] QString i18nDescription() const;
    void setI18nDescription(const QString &newI18nDescription);

    [[nodiscard]] QString i18nLabel() const;
    void setI18nLabel(const QString &newI18nLabel);

    [[nodiscard]] QString id() const;
    void setId(const QString &newId);

    [[nodiscard]] QVariant packageValue() const;
    void setPackageValue(const QVariant &newPackageValue);

    [[nodiscard]] SettingType settingType() const;
    void setSettingType(SettingType newSettingType);

    [[nodiscard]] bool operator==(const ApplicationsSettingsSettingsInfo &other) const;

    [[nodiscard]] QMap<QString, QString> values() const;
    void setValues(const QMap<QString, QString> &newValues);

    [[nodiscard]] bool required() const;
    void setRequired(bool newRequired);

    [[nodiscard]] QVariant value() const;
    void setValue(const QVariant &newValue);

    [[nodiscard]] bool multiLine() const;
    void setMultiLine(bool newMultiLine);

    [[nodiscard]] QJsonObject jsonObj() const;

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT ApplicationsSettingsSettingsInfo::SettingType convertStringToType(const QString &str);
    QJsonObject mJsonObj;
    QString mI18nDescription;
    QString mI18nLabel;
    QString mId;
    QVariant mPackageValue;
    QVariant mValue;
    SettingType mSettingType = SettingType::Unknown;
    // <key, i18nLabel>
    QMap<QString, QString> mValues;
    bool mRequired = false;
    bool mMultiLine = false;
};
QT_DECL_METATYPE_EXTERN_TAGGED(ApplicationsSettingsSettingsInfo, Ruqola_ApplicationsSettingsSettingsInfo, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(ApplicationsSettingsSettingsInfo, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ApplicationsSettingsSettingsInfo &t);
