/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "customsoundsmanager.h"
#include "ruqola_custom_sounds_debug.h"
#include <KLocalizedString>
#include <QJsonArray>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
CustomSoundsManager::CustomSoundsManager(QObject *parent)
    : QObject{parent}
{
}

CustomSoundsManager::~CustomSoundsManager() = default;

void CustomSoundsManager::initializeDefaultSounds()
{
    QList<CustomSoundInfo> listSounds;
    {
        CustomSoundInfo info;
        info.setName(i18n("None"));
        info.setDefaultSound(true);
        info.setIdentifier(QByteArrayLiteral("none"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setName(i18n("Default"));
        info.setDefaultSound(true);
        info.setIdentifier(QByteArrayLiteral("default"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setDefaultSound(true);
        info.setIdentifier(QByteArrayLiteral("chime"));
        info.setName(i18n("Chime"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setDefaultSound(true);
        info.setIdentifier(QByteArrayLiteral("door"));
        info.setName(i18n("Door"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setDefaultSound(true);
        info.setIdentifier(QByteArrayLiteral("beep"));
        info.setName(i18n("Beep"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setDefaultSound(true);
        info.setIdentifier(QByteArrayLiteral("chelle"));
        info.setName(i18n("Chelle"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setDefaultSound(true);
        info.setIdentifier(QByteArrayLiteral("ding"));
        info.setName(i18n("Ding"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(QStringLiteral("mp3"));
        info.setDefaultSound(true);
        info.setIdentifier(QByteArrayLiteral("droplet"));
        info.setName(QStringLiteral("Droplet"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(QStringLiteral("mp3"));
        info.setIdentifier(QByteArrayLiteral("highbell"));
        info.setName(i18n("Highbell"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(QStringLiteral("mp3"));
        info.setIdentifier(QByteArrayLiteral("seasons"));
        info.setName(i18n("Seasons"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(QStringLiteral("mp3"));
        info.setName(i18n("Chime"));
        info.setIdentifier(QByteArrayLiteral("chime"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(QStringLiteral("mp3"));
        info.setName(i18n("Door"));
        info.setIdentifier(QByteArrayLiteral("door"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(QStringLiteral("mp3"));
        info.setName(i18n("Telephone"));
        info.setIdentifier(QByteArrayLiteral("telephone"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(QStringLiteral("mp3"));
        info.setName(i18n("Outbound Call Ringing"));
        info.setIdentifier(QByteArrayLiteral("outbound-call-ringing"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(QStringLiteral("mp3"));
        info.setName(i18n("Call Ended"));
        info.setIdentifier(QByteArrayLiteral("call-ended"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(QStringLiteral("mp3"));
        info.setName(i18n("Dialtone"));
        info.setIdentifier(QByteArrayLiteral("dialtone"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(QStringLiteral("mp3"));
        info.setName(i18n("Ringtone"));
        info.setIdentifier(QByteArrayLiteral("ringtone"));
        listSounds.append(std::move(info));
    }

    setCustomSoundsInfo(listSounds);
    // TODO download it in customsound repo I think.

    // https://<server_url>/sounds/ding.mp3?cacheKey=6.7.2
}

const QList<CustomSoundInfo> &CustomSoundsManager::customSoundsInfo() const
{
    return mCustomSoundsInfo;
}

void CustomSoundsManager::setCustomSoundsInfo(const QList<CustomSoundInfo> &newCustomSoundsInfo)
{
    mCustomSoundsInfo = newCustomSoundsInfo;
    qCDebug(RUQOLA_CUSTOMSOUNDS_LOG) << " Assign Custom Sounds count: " << mCustomSoundsInfo.count();
    Q_EMIT customSoundChanged();
}

void CustomSoundsManager::parseCustomSounds(const QJsonArray &replyArray)
{
    mCustomSoundsInfo.clear();
    initializeDefaultSounds();
    for (int i = 0, total = replyArray.count(); i < total; ++i) {
        CustomSoundInfo info;
        info.parseCustomSoundInfo(replyArray.at(i).toObject());
        if (info.isValid()) {
            mCustomSoundsInfo.append(info);
        }
    }
    qCDebug(RUQOLA_CUSTOMSOUNDS_LOG) << " Parse Custom Sounds count: " << mCustomSoundsInfo.count();
    Q_EMIT customSoundChanged();
    // Download it.
}

void CustomSoundsManager::deleteCustomSounds(const QJsonArray &replyArray)
{
    const auto count{replyArray.count()};
    for (auto i = 0; i < count; ++i) {
        const QJsonObject obj = replyArray.at(i).toObject();
        const QJsonObject emojiData = obj.value("soundData"_L1).toObject();
        const QByteArray identifier = emojiData.value("_id"_L1).toString().toLatin1();
        if (!identifier.isEmpty()) {
            for (int j = 0, total = mCustomSoundsInfo.count(); j < total; ++j) {
                if (mCustomSoundsInfo.at(j).identifier() == identifier) {
                    mCustomSoundsInfo.removeAt(j);
                    Q_EMIT customSoundRemoved(identifier);
                    Q_EMIT customSoundChanged();
                    // TODO remove from disk
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
        const QJsonObject emojiData = obj.value("soundData"_L1).toObject();
        const QByteArray identifier = emojiData.value("_id"_L1).toString().toLatin1();
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
                        Q_EMIT customSoundChanged();
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
                    Q_EMIT customSoundChanged();
                }
            }
        }
    }
    qCDebug(RUQOLA_CUSTOMSOUNDS_LOG) << " Update Custom Sounds count: " << mCustomSoundsInfo.count();
}

QString CustomSoundsManager::soundFile(const QByteArray &identifier) const
{
    // TODO
    return {};
}

#include "moc_customsoundsmanager.cpp"
