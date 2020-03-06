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

#include "command.h"

Command::Command()
{

}

QStringList Command::params() const
{
    return mParams;
}

void Command::setParams(const QStringList &params)
{
    mParams = params;
}

QString Command::commandName() const
{
    return mCommandName;
}

void Command::setCommandName(const QString &commandName)
{
    mCommandName = commandName;
}

QString Command::description() const
{
    return mDescription;
}

void Command::setDescription(const QString &description)
{
    mDescription = description;
}

bool Command::clientOnly() const
{
    return mClientOnly;
}

void Command::setClientOnly(bool clientOnly)
{
    mClientOnly = clientOnly;
}

void Command::parseCommand(const QJsonObject &obj)
{
    mProvidesPreview = obj.value(QStringLiteral("providesPreview")).toBool();
    mClientOnly = obj.value(QStringLiteral("clientOnly")).toBool();
    mCommandName = obj.value(QStringLiteral("command")).toString();
    mDescription = obj.value(QStringLiteral("description")).toString();

    //TODO params
    //TODO permission
    //qDebug() << " mCommandName" << mCommandName;
}

bool Command::operator ==(const Command &other) const
{
    return mParams == other.params() &&
            mCommandName == other.commandName() &&
            mDescription == other.description() &&
            mClientOnly == other.clientOnly() &&
            mProvidesPreview == other.providesPreview();
}

bool Command::isValid() const
{
    return !mCommandName.isEmpty();
}

bool Command::providesPreview() const
{
    return mProvidesPreview;
}

void Command::setProvidesPreview(bool providesPreview)
{
    mProvidesPreview = providesPreview;
}

QStringList Command::permissions() const
{
    return mPermissions;
}

void Command::setPermissions(const QStringList &permissions)
{
    mPermissions = permissions;
}

QDebug operator <<(QDebug d, const Command &t)
{
    d << " mParams : " << t.params();
    d << " mCommandName : " << t.commandName();
    d << " mDescription : " << t.description();
    d << " mClientOnly : " << t.clientOnly();
    d << " mProvidesPreview : " << t.providesPreview();
    return d;
}
