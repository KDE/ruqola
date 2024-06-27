/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "whatsnewtranslationbase.h"

class WhatsNewTranslations : public WhatsNewTranslationsBase
{
public:
    WhatsNewTranslations();
    ~WhatsNewTranslations() override;

    [[nodiscard]] QList<WhatsNewInfo> createWhatsNewInfo() const override;
    [[nodiscard]] QList<KLazyLocalizedString> lastNewFeatures() const override;
};
