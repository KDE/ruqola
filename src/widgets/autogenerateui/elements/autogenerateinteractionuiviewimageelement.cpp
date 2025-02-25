/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewimageelement.h"

AutoGenerateInteractionUiViewImageElement::AutoGenerateInteractionUiViewImageElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewImageElement::~AutoGenerateInteractionUiViewImageElement() = default;

QWidget *AutoGenerateInteractionUiViewImageElement::generateWidget(QWidget *parent)
{
    // TODO
    return nullptr;
}

bool AutoGenerateInteractionUiViewImageElement::operator==(const AutoGenerateInteractionUiViewImageElement &other) const
{
    // TODO
    return other.image() == image();
}

void AutoGenerateInteractionUiViewImageElement::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewActionable::parse(json);
    mImage.parse(json);
}

AutoGenerateInteractionUiViewImage AutoGenerateInteractionUiViewImageElement::image() const
{
    return mImage;
}

void AutoGenerateInteractionUiViewImageElement::setImage(const AutoGenerateInteractionUiViewImage &newImage)
{
    mImage = newImage;
}
