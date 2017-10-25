/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

#ifndef RUQOLAABOUTDATA_H
#define RUQOLAABOUTDATA_H

#include <QObject>
#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT RuqolaAboutData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString version READ version CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString licenses READ licenses CONSTANT)
    Q_PROPERTY(QString about READ about CONSTANT)
    Q_PROPERTY(QString libraries READ libraries CONSTANT)
public:
    explicit RuqolaAboutData(QObject *parent = nullptr);
    ~RuqolaAboutData();

    QString version() const;
    QString title() const;
    QString licenses() const;
    QString about() const;
    QString libraries() const;


    Q_INVOKABLE QString licenseText(const QString &val);
};

#endif // RUQOLAABOUTDATA_H
