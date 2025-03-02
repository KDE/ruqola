/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "autogenerateinteractionuiviewlinearscaleelement.h"
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewLinearScaleElement::AutoGenerateInteractionUiViewLinearScaleElement()
    : AutoGenerateInteractionUiViewActionable()
{
}

AutoGenerateInteractionUiViewLinearScaleElement::~AutoGenerateInteractionUiViewLinearScaleElement() = default;

QWidget *AutoGenerateInteractionUiViewLinearScaleElement::generateWidget(RocketChatAccount *account, QWidget *parent)
{
    Q_UNUSED(account)
    Q_UNUSED(parent)
    // TODO
    return nullptr;
}

bool AutoGenerateInteractionUiViewLinearScaleElement::operator==(const AutoGenerateInteractionUiViewLinearScaleElement &other) const
{
    return AutoGenerateInteractionUiViewActionable::operator==(other);
}

void AutoGenerateInteractionUiViewLinearScaleElement::parseElement(const QJsonObject &json)
{
}

void AutoGenerateInteractionUiViewLinearScaleElement::serializeElement(QJsonObject &o) const
{
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewLinearScaleElement &t)
{
    d.space() << "AutoGenerateInteractionUiViewActionable:" << static_cast<const AutoGenerateInteractionUiViewActionable &>(t);
    return d;
}
