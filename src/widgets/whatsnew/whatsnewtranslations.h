/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "config-ruqola.h"
#include "libruqolawidgets_private_export.h"
#include <TextAddonsWidgets/WhatsNewTranslationsBase>
class LIBRUQOLAWIDGETS_TESTS_EXPORT WhatsNewTranslations : public TextAddonsWidgets::WhatsNewTranslationsBase
{
public:
    WhatsNewTranslations();
    ~WhatsNewTranslations() override;

    [[nodiscard]] QList<TextAddonsWidgets::WhatsNewInfo> createWhatsNewInfo() const override;
    [[nodiscard]] QList<KLazyLocalizedString> lastNewFeatures() const override;
};
