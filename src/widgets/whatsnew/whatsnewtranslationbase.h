/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

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
    [[nodiscard]] virtual QList<KLazyLocalizedString> lastNewFeatures() const = 0;
    [[nodiscard]] QString newFeaturesMD5();
};
