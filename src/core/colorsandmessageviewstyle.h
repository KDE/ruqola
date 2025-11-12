/*
   SPDX-FileCopyrightText: 2022 Nicolas Fella <nicolas.fella@gmx.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "config-ruqola.h"
#include <QObject>
#include <TextUtils/TextUtilsColorsAndMessageViewStyle>

#include <KColorScheme>

#include "libruqolacore_export.h"

class LIBRUQOLACORE_EXPORT ColorsAndMessageViewStyle : public TextUtils::TextUtilsColorsAndMessageViewStyle
{
    Q_OBJECT
public:
    static ColorsAndMessageViewStyle &self();

    ColorsAndMessageViewStyle();
};
