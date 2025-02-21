/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QJsonObject>

class AutoGenerateInteractionUiViewElement
{
public:
    AutoGenerateInteractionUiViewElement();
    ~AutoGenerateInteractionUiViewElement();
    void parseElement(const QJsonObject &json);
};
