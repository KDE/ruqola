/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "blocks.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(Blocks, Ruqola_Blocks)

#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>
#include <algorithm>

using namespace Qt::Literals::StringLiterals;
Blocks::Blocks()

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

const QList<Block> &Blocks::blocks() const
{
    return mBlocks;
}

void Blocks::setVideoConferenceInfo(const VideoConferenceInfo &info)
{
    auto it = std::find_if(mBlocks.begin(), mBlocks.end(), [&info](const Block &block) {
        return block.blockId() == info.blockId();
    });
    if (it != mBlocks.end()) {
        it->setVideoConferenceInfo(info);
    }
}

void Blocks::parseBlocks(const QJsonArray &blocks)
{
    // qDebug() << "blocks ************************************************* " << blocks;
    mBlocks.clear();
    const int total = blocks.count();
    mBlocks.reserve(total);
    for (int i = 0; i < total; ++i) {
        const QJsonObject blockObject = blocks.at(i).toObject();
        Block b;
        b.parseBlock(blockObject);
        if (b.isValid()) {
            mBlocks.append(std::move(b));
        } else {
            qWarning() << " Invalid b " << blockObject;
        }
    }
    // qDebug() << "Blocks::parseBlocks " << mBlocks;
}

bool Blocks::operator==(const Blocks &other) const
{
    return mBlocks == other.mBlocks;
}

QDebug operator<<(QDebug d, const Blocks &t)
{
    for (const Block &block : t.blocks()) {
        d.space() << block << "\n";
    }
    return d;
}

QJsonArray Blocks::serialize(const Blocks &blocks)
{
    QJsonArray blockArray;
    for (const auto &blockInfo : blocks.blocks()) {
        blockArray.append(Block::serialize(blockInfo));
    }
    return blockArray;
}

std::unique_ptr<Blocks> Blocks::deserialize(const QJsonArray &blocksArray)
{
    QList<Block> blocks;
    const int blocksArrayCount = blocksArray.count();
    blocks.reserve(blocksArrayCount);
    for (int i = 0; i < blocksArrayCount; ++i) {
        Block block = Block::deserialize(blocksArray.at(i).toObject());
        blocks.append(std::move(block));
    }
    auto final = std::make_unique<Blocks>();
    final->setBlocks(blocks);
    return final;
}

bool Blocks::isEmpty() const
{
    return mBlocks.isEmpty();
}
