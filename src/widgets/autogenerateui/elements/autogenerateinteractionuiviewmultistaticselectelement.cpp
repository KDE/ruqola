/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewmultistaticselectelement.h"

AutoGenerateInteractionUiViewMultiStaticSelectElement::AutoGenerateInteractionUiViewMultiStaticSelectElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewMultiStaticSelectElement::~AutoGenerateInteractionUiViewMultiStaticSelectElement()
{
}

void AutoGenerateInteractionUiViewMultiStaticSelectElement::parse(const QJsonObject &json)
{
}

QWidget *AutoGenerateInteractionUiViewMultiStaticSelectElement::generateWidget(QWidget *parent)
{
    return nullptr;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewMultiStaticSelectElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<AutoGenerateInteractionUiViewActionable>(t);
    return d;
}
