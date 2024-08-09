/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blocks.h"

#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
Blocks::Blocks()
    : QSharedData()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " Blocks created " << this;
}

Blocks::Blocks(const Blocks &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " Blocks(const Blocks &other) created " << this;
    mBlocks = other.blocks();
}

Blocks::~Blocks()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " Blocks deleted " << this;
}

void Blocks::setBlocks(const QList<Block> &blocks)
{
    mBlocks = blocks;
}

QList<Block> Blocks::blocks() const
{
    return mBlocks;
}

void Blocks::parseBlocks(const QJsonArray &blocks)
{
    mBlocks.clear();
    for (int i = 0, total = blocks.count(); i < total; ++i) {
        const QJsonObject blockObject = blocks.at(i).toObject();
        Block b;
        b.parseBlock(blockObject);
        if (b.isValid()) {
            mBlocks.append(std::move(b));
        }
    }
}

bool Blocks::operator==(const Blocks &other) const
{
    return mBlocks == other.blocks();
}

QDebug operator<<(QDebug d, const Blocks &t)
{
    for (int i = 0; i < t.blocks().count(); i++) {
        d.space() << t.blocks().at(i) << "\n";
    }
    return d;
}

QJsonObject Blocks::serialize(const Blocks &blocks)
{
    QJsonObject obj;
    QJsonArray blockArray;
    const int nBlocks = blocks.count();
    for (int i = 0; i < nBlocks; ++i) {
        blockArray.append(Block::serialize(blocks.at(i)));
    }
    obj["blocks"_L1] = blockArray;
    // TODO
    return obj;
}

Blocks *Blocks::deserialize(const QJsonObject &o)
{
    const QJsonArray blocksArray = o.value("blocks"_L1).toArray();
    for (int i = 0, total = blocksArray.count(); i < total; ++i) {
        const Block block = Block::deserialize(blocksArray.at(i).toObject());
        // TODO message.mBlocks.append(std::move(block));
    }
    // TODO
    return {};
}

bool Blocks::isEmpty() const
{
    return mBlocks.isEmpty();
}
