/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "runninganimatedimage.h"

#include <QMovie>

RunningAnimatedImage::RunningAnimatedImage(const QModelIndex &idx, const QByteArray &identifier)
    : index(idx)
    , identifier(identifier)
    , movie(new QMovie)
{
}

RunningAnimatedImage::~RunningAnimatedImage()
{
    // Note that this happens (with a nullptr movie) when the vector is re-allocated
    delete movie;
}

RunningAnimatedImage::RunningAnimatedImage(RunningAnimatedImage &&other) noexcept
    : index(other.index)
    , identifier(std::move(other.identifier))
    , movie(other.movie)
{
    other.movie = nullptr;
}

RunningAnimatedImage &RunningAnimatedImage::operator=(RunningAnimatedImage &&other)
{
    if (this != &other) {
        index = other.index;
        identifier = std::move(other.identifier);
        // Don't leak the movie we own: std::vector::erase() move-assigns the following elements over the
        // erased one, so this is what destroys the movie of a removed (non-last) entry.
        delete movie;
        movie = other.movie;
        other.movie = nullptr;
    }
    return *this;
}
