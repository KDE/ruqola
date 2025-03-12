/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewimageblock.h"
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewImageBlock::AutoGenerateInteractionUiViewImageBlock(QObject *parent)
    : AutoGenerateInteractionUiViewBlockBase(parent)
{
}
AutoGenerateInteractionUiViewImageBlock::~AutoGenerateInteractionUiViewImageBlock() = default;

QWidget *AutoGenerateInteractionUiViewImageBlock::generateWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

void AutoGenerateInteractionUiViewImageBlock::parseBlock(const QJsonObject &json)
{
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

void AutoGenerateInteractionUiViewImageBlock::serializeBlock(QJsonObject &o) const
{
    o["image"_L1] = mImage.serialize();
}

#include "moc_autogenerateinteractionuiviewimageblock.cpp"
