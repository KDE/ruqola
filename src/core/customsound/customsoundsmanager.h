/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

    [[nodiscard]] const QVector<CustomSoundInfo> &customSoundsInfo() const;
    void setCustomSoundsInfo(const QVector<CustomSoundInfo> &newCustomSoundsInfo);

    void parseCustomSounds(const QJsonArray &obj);

    void deleteCustomSounds(const QJsonArray &replyArray);

    void updateCustomSounds(const QJsonArray &replyArray);

    void initializeDefaultSounds();
Q_SIGNALS:
    void customSoundRemoved(const QString &identifier);
    void customSoundAdded(const QString &identifier);
    void customSoundUpdated(const QString &identifier);

private:
    QVector<CustomSoundInfo> mCustomSoundsInfo;
};
