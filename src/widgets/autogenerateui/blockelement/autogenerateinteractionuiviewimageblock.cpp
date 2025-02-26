/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewimageblock.h"

AutoGenerateInteractionUiViewImageBlock::AutoGenerateInteractionUiViewImageBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewImageBlock::~AutoGenerateInteractionUiViewImageBlock() = default;

QWidget *AutoGenerateInteractionUiViewImageBlock::generateWidget(QWidget *parent) const
{
    return nullptr;
}

void AutoGenerateInteractionUiViewImageBlock::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewBlockBase::parse(json);
    mImage.parse(json);
    // TODO
}

AutoGenerateInteractionUiViewImage AutoGenerateInteractionUiViewImageBlock::image() const
{
    return mImage;
}

void AutoGenerateInteractionUiViewImageBlock::setImage(const AutoGenerateInteractionUiViewImage &newImage)
{
    mImage = newImage;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewImageBlock &t)
{
    d.space() << "image:" << t.image();
    return d;
}

bool AutoGenerateInteractionUiViewImageBlock::operator==(const AutoGenerateInteractionUiViewImageBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other) && other.image() == image();
}
