/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "aitextmanager.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <QRegularExpression>

AiTextManager::AiTextManager(QObject *parent)
    : QObject{parent}
{
}

AiTextManager::~AiTextManager() = default;

QList<AiTextInfo> AiTextManager::textInfos() const
{
    return mTextInfos;
}

void AiTextManager::setTextInfos(const QList<AiTextInfo> &newTextInfos)
{
    mTextInfos = newTextInfos;
}

void AiTextManager::load()
{
    QList<AiTextInfo> infos;
    KSharedConfig::Ptr config;
    const QStringList keyGroups = keyRecorderList(config);

    for (const QString &groupName : keyGroups) {
        KConfigGroup group = config->group(groupName);
        AiTextInfo info;
        info.setRequestText(group.readEntry(QStringLiteral("RequestedText")));
        info.setEnabled(group.readEntry(QStringLiteral("Enabled"), true));
        infos.append(std::move(info));
    }
    setTextInfos(infos);
}

void AiTextManager::save()
{
    KSharedConfig::Ptr config;
    const QStringList filterGroups = keyRecorderList(config);

    for (const QString &group : filterGroups) {
        config->deleteGroup(group);
    }
    for (int i = 0, total = mTextInfos.count(); i < total; ++i) {
        const QString groupName = QStringLiteral("AskIA #%1").arg(i);
        KConfigGroup group = config->group(groupName);
        const AiTextInfo &info = mTextInfos.at(i);
        group.writeEntry(QStringLiteral("RequestedText"), info.requestText());
        group.writeEntry(QStringLiteral("Enabled"), info.enabled());
    }
    config->sync();
}

QStringList AiTextManager::keyRecorderList(KSharedConfig::Ptr &config) const
{
    config = KSharedConfig::openConfig();
    const QStringList keyGroups = config->groupList().filter(QRegularExpression(QStringLiteral("AskIA #\\d+")));
    return keyGroups;
}

#include "moc_aitextmanager.cpp"
