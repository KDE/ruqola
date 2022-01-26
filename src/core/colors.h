/*
   SPDX-FileCopyrightText: 2022 Nicolas Fella <nicolas.fella@gmx.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

#include <KColorScheme>

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT Colors : public QObject
{
    Q_OBJECT
public:
    static Colors &self();

    Colors();

    Q_REQUIRED_RESULT KColorScheme scheme() const;

private:
    KColorScheme mScheme;
};
