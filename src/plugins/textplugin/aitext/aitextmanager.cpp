/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "aitextmanager.h"

AiTextManager::AiTextManager(QObject *parent)
    : QObject{parent}
{
}

AiTextManager::~AiTextManager() = default;

void AiTextManager::load()
{
    // TODO use KConfig
}

void AiTextManager::save()
{
    // TODO use KConfig
}

QList<AiTextInfo> AiTextManager::textInfos() const
{
    return mTextInfos;
}

void AiTextManager::setTextInfos(const QList<AiTextInfo> &newTextInfos)
{
    mTextInfos = newTextInfos;
}

#include "moc_aitextmanager.cpp"
