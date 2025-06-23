/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

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
        info.setIdentifier("none"_ba);
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(u"mp3"_s);
        info.setName(i18n("Default"));
        info.setDefaultSound(true);
        info.setIdentifier("default"_ba);
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(u"mp3"_s);
        info.setDefaultSound(true);
        info.setIdentifier("chime"_ba);
        info.setName(i18n("Chime"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(u"mp3"_s);
        info.setDefaultSound(true);
        info.setIdentifier("door"_ba);
        info.setName(i18n("Door"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(u"mp3"_s);
        info.setDefaultSound(true);
        info.setIdentifier("beep"_ba);
        info.setName(i18n("Beep"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(u"mp3"_s);
        info.setDefaultSound(true);
        info.setIdentifier("chelle"_ba);
        info.setName(i18n("Chelle"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(u"mp3"_s);
        info.setDefaultSound(true);
        info.setIdentifier("ding"_ba);
        info.setName(i18n("Ding"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setExtension(u"mp3"_s);
        info.setDefaultSound(true);
        info.setIdentifier("droplet"_ba);
        info.setName(u"Droplet"_s);
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(u"mp3"_s);
        info.setIdentifier("highbell"_ba);
        info.setName(i18n("Highbell"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(u"mp3"_s);
        info.setIdentifier("seasons"_ba);
        info.setName(i18n("Seasons"));
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(u"mp3"_s);
        info.setName(i18n("Chime"));
        info.setIdentifier("chime"_ba);
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(u"mp3"_s);
        info.setName(i18n("Door"));
        info.setIdentifier("door"_ba);
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(u"mp3"_s);
        info.setName(i18n("Telephone"));
        info.setIdentifier("telephone"_ba);
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(u"mp3"_s);
        info.setName(i18n("Outbound Call Ringing"));
        info.setIdentifier("outbound-call-ringing"_ba);
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(u"mp3"_s);
        info.setName(i18n("Call Ended"));
        info.setIdentifier("call-ended"_ba);
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(u"mp3"_s);
        info.setName(i18n("Dialtone"));
        info.setIdentifier("dialtone"_ba);
        listSounds.append(std::move(info));
    }
    {
        CustomSoundInfo info;
        info.setDefaultSound(true);
        info.setExtension(u"mp3"_s);
        info.setName(i18n("Ringtone"));
        info.setIdentifier("ringtone"_ba);
        listSounds.append(std::move(info));
    }

    setCustomSoundsInfo(listSounds);
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
                    const QString soundPath = soundFilePath(identifier);
                    mCustomSoundsInfo.removeAt(j);
                    Q_EMIT customSoundRemoved(identifier, soundPath);
                    Q_EMIT customSoundChanged();
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
                        mCustomSoundsInfo.append(std::move(sound));
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

QString CustomSoundsManager::soundFilePath(const QByteArray &identifier) const
{
    auto index = std::find_if(mCustomSoundsInfo.begin(), mCustomSoundsInfo.end(), [identifier](const CustomSoundInfo &info) {
        return (info.identifier() == identifier);
    });
    QString url;
    if (index != mCustomSoundsInfo.end()) {
        url = (*index).generateUrl();
    }
    return url;
}

#include "moc_customsoundsmanager.cpp"
