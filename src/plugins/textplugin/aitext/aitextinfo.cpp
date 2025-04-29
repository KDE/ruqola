/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextinfo.h"

AiTextInfo::AiTextInfo() = default;

AiTextInfo::~AiTextInfo() = default;

bool AiTextInfo::enabled() const
{
    return mEnabled;
}

void AiTextInfo::setEnabled(bool newEnabled)
{
    mEnabled = newEnabled;
}

QDebug operator<<(QDebug d, const AiTextInfo &t)
{
    d.space() << "enabled:" << t.enabled();
    return d;
}
