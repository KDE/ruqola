/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewimageelement.h"
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewImageElement::AutoGenerateInteractionUiViewImageElement(QObject *parent)
    : AutoGenerateInteractionUiViewActionable(parent)
{
}

AutoGenerateInteractionUiViewImageElement::~AutoGenerateInteractionUiViewImageElement() = default;

QWidget *AutoGenerateInteractionUiViewImageElement::generateWidget(RocketChatAccount *account, QWidget *parent)
{
    Q_UNUSED(account)
    Q_UNUSED(parent)
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
    // TODO verify it
    o["image"_L1] = mImage.serialize();
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewImageElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    d.space() << "image:" << t.image();
    return d;
}

#include "moc_autogenerateinteractionuiviewimageelement.cpp"
