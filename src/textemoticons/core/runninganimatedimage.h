/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "textemoticonscore_export.h"

#include <QPersistentModelIndex>
class QMovie;
namespace TextEmoticonsCore
{
struct TEXTEMOTICONSCORE_EXPORT RunningAnimatedImage {
    explicit RunningAnimatedImage(const QModelIndex &idx);
    ~RunningAnimatedImage();
    RunningAnimatedImage(const RunningAnimatedImage &) = delete;
    RunningAnimatedImage(RunningAnimatedImage &&other) noexcept;
    RunningAnimatedImage &operator=(const RunningAnimatedImage &) = delete;
    RunningAnimatedImage &operator=(RunningAnimatedImage &&other);

    QPersistentModelIndex index;
    QMovie *movie = nullptr;
};
}
