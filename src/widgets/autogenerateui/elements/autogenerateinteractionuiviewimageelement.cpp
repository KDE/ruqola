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
    return other.image() == image() && AutoGenerateInteractionUiViewActionable::operator==(other);
}

void AutoGenerateInteractionUiViewImageElement::parseElement(const QJsonObject &json)
{
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

void AutoGenerateInteractionUiViewImageElement::serializeElement(QJsonObject &o) const
{
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewImageElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "image:" << t.image();
    return d;
}
