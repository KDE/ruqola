/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autotranslatelanguage.h"
#include "libruqola_private_export.h"
#include <QDebug>
#include <QList>

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

Q_DECLARE_METATYPE(AutotranslateLanguages)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutotranslateLanguages &t);
