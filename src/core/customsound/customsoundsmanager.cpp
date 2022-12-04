/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundsmanager.h"
#include "ruqola_custom_sounds_debug.h"
#include <QJsonArray>

CustomSoundsManager::CustomSoundsManager(QObject *parent)
    : QObject{parent}
{
}

CustomSoundsManager::~CustomSoundsManager() = default;

void CustomSoundsManager::initializeDefaultSounds()
{
    QVector<CustomSoundInfo> listSounds;
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setIdentifier(QStringLiteral("chime"));
        info.setName(QStringLiteral("Chime"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setIdentifier(QStringLiteral("door"));
        info.setName(QStringLiteral("Door"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setIdentifier(QStringLiteral("beep"));
        info.setName(QStringLiteral("Beep"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setIdentifier(QStringLiteral("chelle"));
        info.setName(QStringLiteral("Chelle"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setIdentifier(QStringLiteral("ding"));
        info.setName(QStringLiteral("Ding"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setIdentifier(QStringLiteral("droplet"));
        info.setName(QStringLiteral("Droplet"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setIdentifier(QStringLiteral("highbell"));
        info.setName(QStringLiteral("Highbell"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setIdentifier(QStringLiteral("seasons"));
        info.setName(QStringLiteral("Seasons"));
        listSounds.append(std::move(info));
    }
    setCustomSoundsInfo(listSounds);
    // TODO download it in customsound repo I think.
}

const QVector<CustomSoundInfo> &CustomSoundsManager::customSoundsInfo() const
{
    return mCustomSoundsInfo;
}

void CustomSoundsManager::setCustomSoundsInfo(const QVector<CustomSoundInfo> &newCustomSoundsInfo)
{
    mCustomSoundsInfo = newCustomSoundsInfo;
    qCDebug(RUQOLA_CUSTOMSOUNDS_LOG) << " Assign Custom Sounds count: " << mCustomSoundsInfo.count();
}

void CustomSoundsManager::parseCustomSounds(const QJsonArray &replyArray)
{
    mCustomSoundsInfo.clear();
    for (int i = 0, total = replyArray.count(); i < total; ++i) {
        CustomSoundInfo info;
        info.parseCustomSoundInfo(replyArray.at(i).toObject());
        if (info.isValid()) {
            mCustomSoundsInfo.append(info);
        }
    }
    qCDebug(RUQOLA_CUSTOMSOUNDS_LOG) << " Parse Custom Sounds count: " << mCustomSoundsInfo.count();
}

void CustomSoundsManager::deleteCustomSounds(const QJsonArray &replyArray)
{
    const auto count{replyArray.count()};
    for (auto i = 0; i < count; ++i) {
        const QJsonObject obj = replyArray.at(i).toObject();
        const QJsonObject emojiData = obj.value(QStringLiteral("soundData")).toObject();
        const QString identifier = emojiData.value(QStringLiteral("_id")).toString();
        if (!identifier.isEmpty()) {
            for (int j = 0, total = mCustomSoundsInfo.count(); j < total; ++j) {
                if (mCustomSoundsInfo.at(j).identifier() == identifier) {
                    mCustomSoundsInfo.removeAt(j);
                    Q_EMIT customSoundRemoved(identifier);
                    break;
                }
            }
        }
    }
    qCDebug(RUQOLA_CUSTOMSOUNDS_LOG) << " Delete Custom Sounds count: " << mCustomSoundsInfo.count();
}

void CustomSoundsManager::updateCustomSounds(const QJsonArray &replyArray)
{
    const auto count{replyArray.count()};
    for (auto i = 0; i < count; ++i) {
        const QJsonObject obj = replyArray.at(i).toObject();
        const QJsonObject emojiData = obj.value(QStringLiteral("soundData")).toObject();
        const QString identifier = emojiData.value(QStringLiteral("_id")).toString();
        if (!identifier.isEmpty()) {
            bool soundIdentifierFound = false;
            for (int j = 0, total = mCustomSoundsInfo.count(); j < total; ++j) {
                if (mCustomSoundsInfo.at(j).identifier() == identifier) {
                    soundIdentifierFound = true;
                    mCustomSoundsInfo.removeAt(j);
                    CustomSoundInfo sound;
                    sound.parseCustomSoundInfo(emojiData);
                    if (sound.isValid()) {
                        mCustomSoundsInfo.append(sound);
                        Q_EMIT customSoundUpdated(identifier);
                    }
                    break;
                }
            }
            if (!soundIdentifierFound) {
                CustomSoundInfo sound;
                sound.parseCustomSoundInfo(emojiData);
                if (sound.isValid()) {
                    mCustomSoundsInfo.append(std::move(sound));
                    Q_EMIT customSoundAdded(identifier);
                }
            }
        }
    }
    qCDebug(RUQOLA_CUSTOMSOUNDS_LOG) << " Update Custom Sounds count: " << mCustomSoundsInfo.count();
}
