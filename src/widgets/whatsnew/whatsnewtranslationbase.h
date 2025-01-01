/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "whatsnewinfo.h"
#include <KLazyLocalizedString>

class WhatsNewTranslationsBase
{
public:
    WhatsNewTranslationsBase();
    virtual ~WhatsNewTranslationsBase();
    [[nodiscard]] virtual QList<WhatsNewInfo> createWhatsNewInfo() const = 0;
    [[nodiscard]] QString newFeaturesMD5() const;

private:
    [[nodiscard]] virtual QList<KLazyLocalizedString> lastNewFeatures() const = 0;
};
