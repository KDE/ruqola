/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiview.h"

AutoGenerateInteractionUiView::AutoGenerateInteractionUiView() = default;

AutoGenerateInteractionUiView::~AutoGenerateInteractionUiView() = default;

void AutoGenerateInteractionUiView::parseView(const QJsonObject &json)
{
    // TODO
}

QByteArray AutoGenerateInteractionUiView::id() const
{
    return mId;
}

void AutoGenerateInteractionUiView::setId(const QByteArray &newId)
{
    mId = newId;
}

bool AutoGenerateInteractionUiView::operator==(const AutoGenerateInteractionUiView &other) const
{
    return other.id() == id();
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiView &t)
{
    d.space() << "id:" << t.id();
    return d;
}
