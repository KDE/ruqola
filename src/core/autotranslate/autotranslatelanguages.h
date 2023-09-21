/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autotranslatelanguage.h"
#include "libruqola_private_export.h"
#include <QDebug>
#include <QVector>

class LIBRUQOLACORE_TESTS_EXPORT AutotranslateLanguages
{
public:
    AutotranslateLanguages();

    [[nodiscard]] QVector<AutotranslateLanguage> autotranslateLanguage() const;
    void setAutotranslateLanguages(const QVector<AutotranslateLanguage> &autotranslateLanguage);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] int count() const;
    void clear();
    [[nodiscard]] AutotranslateLanguage at(int index) const;

    void parseLanguages(const QJsonObject &obj);

private:
    QVector<AutotranslateLanguage> mAutotranslateLanguages;
};

Q_DECLARE_METATYPE(AutotranslateLanguages)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutotranslateLanguages &t);
