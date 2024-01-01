/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT Command
{
public:
    Command();

    [[nodiscard]] QString params() const;
    void setParams(const QString &params);

    [[nodiscard]] QString commandName() const;
    void setCommandName(const QString &commandName);

    [[nodiscard]] QString description() const;
    void setDescription(const QString &description);

    [[nodiscard]] bool clientOnly() const;
    void setClientOnly(bool clientOnly);

    void parseCommand(const QJsonObject &obj);

    [[nodiscard]] bool operator==(const Command &other) const;

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] bool providesPreview() const;
    void setProvidesPreview(bool providesPreview);

    [[nodiscard]] QStringList permissions() const;
    void setPermissions(const QStringList &permissions);

    [[nodiscard]] QString translatedDescription() const;
    [[nodiscard]] QString translatedParams() const;

private:
    void convertDescriptionI18n();
    void convertParamsI18n();
    QStringList mPermissions;
    QString mParams;
    QString mCommandName;
    QString mDescription;
    QString mTranslatedDescription;
    QString mTranslatedParams;
    bool mProvidesPreview = false;
    bool mClientOnly = false;
};
Q_DECLARE_METATYPE(Command)
Q_DECLARE_TYPEINFO(Command, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Command &t);
