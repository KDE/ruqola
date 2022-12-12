/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "block.h"

Block::Block() = default;

void Block::parseBlock(const QJsonObject &block)
{
    // TODO
}

bool Block::operator==(const Block &other) const
{
    // TODO
    return true;
}

QJsonObject Block::serialize(const Block &block)
{
    // TODO
    return {};
}

Block Block::fromJSon(const QJsonObject &o)
{
    // TODO
    return {};
}

QDebug operator<<(QDebug d, const Block &t)
{
    // TODO
    return d;
}
