/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include "customsoundinfo.h"
#include "libruqolacore_export.h"
#include <QObject>

class LIBRUQOLACORE_EXPORT CustomSoundsManager : public QObject
{
    Q_OBJECT
public:
    explicit CustomSoundsManager(QObject *parent = nullptr);
    ~CustomSoundsManager() override;

    Q_REQUIRED_RESULT const QVector<CustomSoundInfo> &customSoundsInfo() const;
    void setCustomSoundsInfo(const QVector<CustomSoundInfo> &newCustomSoundsInfo);

    void parseCustomSounds(const QJsonArray &obj);

    void deleteCustomSounds(const QJsonArray &replyArray);

    void updateCustomSounds(const QJsonArray &replyArray);

Q_SIGNALS:
    void customSoundRemoved(const QString &identifier);
    void customSoundAdded(const QString &identifier);
    void customSoundUpdated(const QString &identifier);
private:
    QVector<CustomSoundInfo> mCustomSoundsInfo;
};
