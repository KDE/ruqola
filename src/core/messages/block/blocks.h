/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "block.h"
#include "libruqolacore_export.h"
#include <QList>
class LIBRUQOLACORE_EXPORT Blocks : public QSharedData
{
public:
    explicit Blocks(const Blocks &other);
    Blocks();
    ~Blocks();

    void setBlocks(const QList<Block> &blocks);
    [[nodiscard]] QList<Block> blocks() const;

    void parseBlocks(const QJsonArray &array);

    [[nodiscard]] bool operator==(const Blocks &other) const;

    [[nodiscard]] static QJsonArray serialize(const Blocks &reactions);
    [[nodiscard]] static Blocks *deserialize(const QJsonArray &o);

    [[nodiscard]] bool isEmpty() const;

    void setVideoConferenceInfo(const VideoConferenceInfo &info);

private:
    QList<Block> mBlocks;
};
QT_DECL_METATYPE_EXTERN_TAGGED(Blocks, Ruqola_Blocks, LIBRUQOLACORE_EXPORT)
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const Blocks &t);
