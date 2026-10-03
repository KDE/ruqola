/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

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

void AutoGenerateInteractionUiViewDividerBlock::setErrorMessages([[maybe_unused]] const QMap<QString, QString> &map)
{
}

void AutoGenerateInteractionUiViewDividerBlock::assignState([[maybe_unused]] const QList<StateInfo> &info)
{
}

void AutoGenerateInteractionUiViewDividerBlock::serializeBlock([[maybe_unused]] QJsonObject &o) const
{
    // Nothing
}

QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> AutoGenerateInteractionUiViewDividerBlock::serializeBlockState() const
{
    return {};
}

void AutoGenerateInteractionUiViewDividerBlock::parseBlock([[maybe_unused]] const QJsonObject &json)
{
    // Nothing
}

#include "moc_autogenerateinteractionuiviewdividerblock.cpp"
