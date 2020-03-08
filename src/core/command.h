/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef COMMAND_H
#define COMMAND_H

#include "libruqola_private_export.h"
#include <QString>
#include <QDebug>
#include <QJsonObject>

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

    Q_REQUIRED_RESULT bool operator ==(const Command &other) const;

    Q_REQUIRED_RESULT bool isValid() const;

    Q_REQUIRED_RESULT bool providesPreview() const;
    void setProvidesPreview(bool providesPreview);

    Q_REQUIRED_RESULT QStringList permissions() const;
    void setPermissions(const QStringList &permissions);

    Q_REQUIRED_RESULT QString translatedDescription() const;
private:
    void convertDescriptionI18n();
    QStringList mPermissions;
    QString mParams;
    QString mCommandName;
    QString mDescription;
    QString mTranslatedDescription;
    bool mProvidesPreview = false;
    bool mClientOnly = false;
};
Q_DECLARE_METATYPE(Command)
Q_DECLARE_TYPEINFO(Command, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const Command &t);

#endif // COMMAND_H
