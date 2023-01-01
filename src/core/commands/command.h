/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>

class LIBRUQOLACORE_TESTS_EXPORT Command
{
public:
    Command();

    Q_REQUIRED_RESULT QString params() const;
    void setParams(const QString &params);

    Q_REQUIRED_RESULT QString commandName() const;
    void setCommandName(const QString &commandName);

    Q_REQUIRED_RESULT QString description() const;
    void setDescription(const QString &description);

    Q_REQUIRED_RESULT bool clientOnly() const;
    void setClientOnly(bool clientOnly);

    void parseCommand(const QJsonObject &obj);

    Q_REQUIRED_RESULT bool operator==(const Command &other) const;

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT bool providesPreview() const;
    void setProvidesPreview(bool providesPreview);

    Q_REQUIRED_RESULT QStringList permissions() const;
    void setPermissions(const QStringList &permissions);

    Q_REQUIRED_RESULT QString translatedDescription() const;
    Q_REQUIRED_RESULT QString translatedParams() const;

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
