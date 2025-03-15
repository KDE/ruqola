/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewdividerblock.h"

#include <KSeparator>

AutoGenerateInteractionUiViewDividerBlock::AutoGenerateInteractionUiViewDividerBlock(QObject *parent)
    : AutoGenerateInteractionUiViewBlockBase(parent)
{
}

AutoGenerateInteractionUiViewDividerBlock::~AutoGenerateInteractionUiViewDividerBlock() = default;

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewDividerBlock &t)
{
    d.space() << "AutoGenerateInteractionUiViewBlockBase:" << static_cast<const AutoGenerateInteractionUiViewBlockBase &>(t);
    return d;
}

bool AutoGenerateInteractionUiViewDividerBlock::operator==(const AutoGenerateInteractionUiViewDividerBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other);
}

QWidget *AutoGenerateInteractionUiViewDividerBlock::generateWidget(QWidget *parent)
{
    return new KSeparator(parent);
}

void AutoGenerateInteractionUiViewDividerBlock::serializeBlock(QJsonObject &o) const
{
    Q_UNUSED(o)
    // Nothing
}

void AutoGenerateInteractionUiViewDividerBlock::serializeBlockState(QJsonObject &obj) const
{
    // TODO
}

void AutoGenerateInteractionUiViewDividerBlock::parseBlock(const QJsonObject &json)
{
    Q_UNUSED(json)
    // Nothing
}

#include "moc_autogenerateinteractionuiviewdividerblock.cpp"
