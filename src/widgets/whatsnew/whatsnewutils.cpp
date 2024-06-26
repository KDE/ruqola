/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewutils.h"
#include <KLocalizedString>

QString WhatsNewUtils::convertVersionEnumToString(WhatsNewUtils::VersionType type)
{
    switch (type) {
    case AllVersion:
        return i18n("All Version");
    case Version2_0:
        return i18n("Version 2.0");
    case Version2_1:
        return i18n("Version 2.1");
    case Version2_2:
        return i18n("Version 2.2");
    case Version2_3:
        return i18n("Version 2.3");
    }
    Q_UNREACHABLE();
    return {};
}
