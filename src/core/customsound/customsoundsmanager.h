/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

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

    [[nodiscard]] const QList<CustomSoundInfo> &customSoundsInfo() const;
    void setCustomSoundsInfo(const QList<CustomSoundInfo> &newCustomSoundsInfo);

    void parseCustomSounds(const QJsonArray &obj);

    void deleteCustomSounds(const QJsonArray &replyArray);

    void updateCustomSounds(const QJsonArray &replyArray);

    void initializeDefaultSounds();

    [[nodiscard]] QString soundFilePath(const QByteArray &) const;

Q_SIGNALS:
    void customSoundRemoved(const QByteArray &identifier, const QString &soundFilePath);
    void customSoundAdded(const QByteArray &identifier);
    void customSoundUpdated(const QByteArray &identifier);
    void customSoundChanged();

private:
    QList<CustomSoundInfo> mCustomSoundsInfo;
};
