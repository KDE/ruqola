/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QObject>

class LIBRUQOLAWIDGETS_TESTS_EXPORT TranslationMenu : public QObject
{
    Q_OBJECT
public:
    explicit TranslationMenu(QObject *parent = nullptr);
    ~TranslationMenu();
};
