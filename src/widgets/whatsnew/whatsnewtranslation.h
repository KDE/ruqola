/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "whatsnewinfo.h"
#include <KLazyLocalizedString>

namespace WhatsNewTranslations
{
[[nodiscard]] QList<WhatsNewInfo> createWhatsNewInfo();
[[nodiscard]] QList<KLazyLocalizedString> lastNewFeatures();
[[nodiscard]] QString newFeaturesMD5();
}
