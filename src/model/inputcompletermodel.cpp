/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "inputcompletermodel.h"
#include "ruqola_debug.h"

#include <QJsonArray>

InputCompleterModel::InputCompleterModel(QObject *parent)
    : QStringListModel(parent)
{
}

InputCompleterModel::~InputCompleterModel()
{
}

void InputCompleterModel::parseInputTextCompleter(const QJsonObject &obj)
{
    QStringList lst;
    const QJsonArray rooms = obj.value(QLatin1String("rooms")).toArray();
    for (int i = 0; i < rooms.size(); i++) {
        const QJsonObject o = rooms.at(i).toObject();
        lst.append(o.value(QLatin1String("name")).toString());
    }
    const QJsonArray users = obj.value(QLatin1String("users")).toArray();
    for (int i = 0; i < users.size(); i++) {
        const QJsonObject o = users.at(i).toObject();
        lst.append(o.value(QLatin1String("name")).toString());
    }
    //TODO
    qDebug() << " void InputCompleterModel::parseInputTextCompleter(const QJsonObject &obj)"<<lst;
    setStringList(lst);
}
