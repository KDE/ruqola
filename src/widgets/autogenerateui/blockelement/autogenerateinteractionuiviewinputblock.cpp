/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewinputblock.h"
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewInputBlock::AutoGenerateInteractionUiViewInputBlock()
    : AutoGenerateInteractionUiViewBlockBase()
{
}

AutoGenerateInteractionUiViewInputBlock::~AutoGenerateInteractionUiViewInputBlock()
{
    delete mElement;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewInputBlock &t)
{
    d.space() << "optional:" << t.optional();
    d.space() << "label:" << t.label();
    // TODO implement debug t
    return d;
}

bool AutoGenerateInteractionUiViewInputBlock::operator==(const AutoGenerateInteractionUiViewInputBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other) && other.optional() == optional() && other.label() == label();
}

void AutoGenerateInteractionUiViewInputBlock::parse(const QJsonObject &json)
{
    AutoGenerateInteractionUiViewBlockBase::parse(json);
    mOptional = json["optional"_L1].toBool();
    mLabel.parse(json["label"_L1].toObject());
    // TODO element
}

bool AutoGenerateInteractionUiViewInputBlock::optional() const
{
    return mOptional;
}

void AutoGenerateInteractionUiViewInputBlock::setOptional(bool newOptional)
{
    mOptional = newOptional;
}

AutoGenerateInteractionUiViewText AutoGenerateInteractionUiViewInputBlock::label() const
{
    return mLabel;
}

void AutoGenerateInteractionUiViewInputBlock::setLabel(const AutoGenerateInteractionUiViewText &newLabel)
{
    mLabel = newLabel;
}

QWidget *AutoGenerateInteractionUiViewInputBlock::generateWidget(QWidget *parent) const
{
    return nullptr;
    // TODO
}
