/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "runninganimatedimage.h"

#include <QMovie>
using namespace TextEmoticonsCore;
RunningAnimatedImage::RunningAnimatedImage(const QModelIndex &idx)
    : index(idx)
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
    , movie(other.movie)
{
    other.movie = nullptr;
}

RunningAnimatedImage &RunningAnimatedImage::operator=(RunningAnimatedImage &&other)
{
    index = other.index;
    movie = other.movie;
    other.movie = nullptr;
    return *this;
}
