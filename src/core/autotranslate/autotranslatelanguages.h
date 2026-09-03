/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autotranslatelanguage.h"
#include "libruqola_private_export.h"
#include <QList>
class QDebug;

class LIBRUQOLACORE_TESTS_EXPORT AutotranslateLanguages
{
public:
    AutotranslateLanguages();

    [[nodiscard]] QList<AutotranslateLanguage> autotranslateLanguage() const;
    void setAutotranslateLanguages(const QList<AutotranslateLanguage> &autotranslateLanguage);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] int count() const;
    void clear();
    [[nodiscard]] AutotranslateLanguage at(int index) const;

    void parseLanguages(const QJsonObject &obj);

private:
    QList<AutotranslateLanguage> mAutotranslateLanguages;
};

QT_DECL_METATYPE_EXTERN_TAGGED(AutotranslateLanguages, Ruqola_AutotranslateLanguages, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutotranslateLanguages &t);
Q_DECLARE_TYPEINFO(AutotranslateLanguages, Q_RELOCATABLE_TYPE);
