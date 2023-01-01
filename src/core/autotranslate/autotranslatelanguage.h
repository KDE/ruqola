/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqola_private_export.h"
#include <QDebug>

class LIBRUQOLACORE_TESTS_EXPORT AutotranslateLanguage
{
public:
    AutotranslateLanguage();
    Q_REQUIRED_RESULT QString language() const;
    void setLanguage(const QString &language);

    Q_REQUIRED_RESULT QString displayLanguage() const;
    void setDisplayLanguage(const QString &displayLanguage);

private:
    QString mLanguage;
    QString mDisplayLanguage;
};
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutotranslateLanguage &t);
Q_DECLARE_METATYPE(AutotranslateLanguage)
Q_DECLARE_TYPEINFO(AutotranslateLanguage, Q_MOVABLE_TYPE);
