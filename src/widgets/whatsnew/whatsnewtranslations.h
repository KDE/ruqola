/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "config-ruqola.h"
#include "libruqolawidgets_private_export.h"
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
#include <TextAddonsWidgets/WhatsNewTranslationsBase>
#else
#include "whatsnewtranslationbase.h"
#endif
#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
class LIBRUQOLAWIDGETS_TESTS_EXPORT WhatsNewTranslations : public TextAddonsWidgets::WhatsNewTranslationsBase
#else
class LIBRUQOLAWIDGETS_TESTS_EXPORT WhatsNewTranslations : public WhatsNewTranslationsBase
#endif
{
public:
    WhatsNewTranslations();
    ~WhatsNewTranslations() override;

#if HAVE_TEXTUTILS_HAS_WHATSNEW_SUPPORT
    [[nodiscard]] QList<TextAddonsWidgets::WhatsNewInfo> createWhatsNewInfo() const override;
#else
    [[nodiscard]] QList<WhatsNewInfo> createWhatsNewInfo() const override;
#endif
    [[nodiscard]] QList<KLazyLocalizedString> lastNewFeatures() const override;
};
