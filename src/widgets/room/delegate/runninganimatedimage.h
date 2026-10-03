/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"

#include <QByteArray>
#include <QPersistentModelIndex>
class QMovie;

struct LIBRUQOLAWIDGETS_TESTS_EXPORT RunningAnimatedImage {
    explicit RunningAnimatedImage(const QModelIndex &idx, const QByteArray &identifier);
    ~RunningAnimatedImage();
    RunningAnimatedImage(const RunningAnimatedImage &) = delete;
    RunningAnimatedImage(RunningAnimatedImage &&other) noexcept;
    RunningAnimatedImage &operator=(const RunningAnimatedImage &) = delete;
    RunningAnimatedImage &operator=(RunningAnimatedImage &&other);

    QPersistentModelIndex index;
    // A single message (i.e. a single model index) can show several animated images at once, so the index
    // alone doesn't identify an animation: the identifier tells them apart (attachment id, reaction name...).
    QByteArray identifier;
    QMovie *movie = nullptr;
};
