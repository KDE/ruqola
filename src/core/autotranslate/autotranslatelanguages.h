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

    Q_REQUIRED_RESULT QVector<AutotranslateLanguage> autotranslateLanguage() const;
    void setAutotranslateLanguages(const QVector<AutotranslateLanguage> &autotranslateLanguage);

    Q_REQUIRED_RESULT bool isEmpty() const;
    Q_REQUIRED_RESULT int count() const;
    void clear();
    Q_REQUIRED_RESULT AutotranslateLanguage at(int index) const;

    void parseLanguages(const QJsonObject &obj);

private:
    QVector<AutotranslateLanguage> mAutotranslateLanguages;
};

Q_DECLARE_METATYPE(AutotranslateLanguages)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const AutotranslateLanguages &t);
