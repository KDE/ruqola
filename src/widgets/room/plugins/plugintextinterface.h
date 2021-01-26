/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#ifndef PLUGINTEXTINTERFACE_H
#define PLUGINTEXTINTERFACE_H

#include "libruqolawidgets_export.h"
#include <QObject>
class QMenu;
class LIBRUQOLAWIDGETS_EXPORT PluginTextInterface : public QObject
{
    Q_OBJECT
public:
    explicit PluginTextInterface(QObject *parent = nullptr);
    ~PluginTextInterface() override;

    virtual void addAction(QMenu *menu) = 0;

    virtual void setSelectedText(const QString &str);
};

#endif // PLUGINTEXTINTERFACE_H
