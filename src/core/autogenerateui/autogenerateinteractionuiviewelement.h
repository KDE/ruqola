/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libruqola_private_export.h"
#include <QJsonObject>

class LIBRUQOLACORE_TESTS_EXPORT AutoGenerateInteractionUiViewElement
{
public:
    AutoGenerateInteractionUiViewElement();
    virtual ~AutoGenerateInteractionUiViewElement();
    virtual void parse(const QJsonObject &json);
};
