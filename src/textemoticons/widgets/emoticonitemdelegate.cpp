/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonitemdelegate.h"
#include <QAbstractItemView>
#include <QMovie>
#include <QPainter>
#include <TextEmoticonsCore/EmojiModel>
using namespace TextEmoticonsWidgets;
EmoticonItemDelegate::EmoticonItemDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

EmoticonItemDelegate::~EmoticonItemDelegate() = default;

void EmoticonItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const bool imageAnimated = index.data(TextEmoticonsCore::EmojiModel::Animated).toBool();
    if (imageAnimated) {
        const QString imageAnimatedFilename = index.data(TextEmoticonsCore::EmojiModel::AnimatedFileName).toString();
        auto it = findRunningAnimatedImage(index);
        QPixmap scaledPixmap;
        if (it != mRunningAnimatedImages.end()) {
            scaledPixmap = (*it).movie->currentPixmap();
        } else {
            mRunningAnimatedImages.emplace_back(index);
            auto &rai = mRunningAnimatedImages.back();
            rai.movie->setFileName(imageAnimatedFilename);
            rai.movie->setScaledSize(option.decorationSize);
            auto view = qobject_cast<QAbstractItemView *>(const_cast<QWidget *>(option.widget));
            const QPersistentModelIndex &idx = rai.index;
            QObject::connect(
                rai.movie,
                &QMovie::frameChanged,
                view,
                [view, idx, this]() {
                    if (view->viewport()->rect().contains(view->visualRect(idx))) {
                        view->update(idx);
                    } else {
                        removeRunningAnimatedImage(idx);
                    }
                },
                Qt::QueuedConnection);
            rai.movie->start();
            scaledPixmap = rai.movie->currentPixmap();
        }
        scaledPixmap.setDevicePixelRatio(option.widget->devicePixelRatioF());
        painter->drawPixmap(QRect(option.rect.topLeft(), option.decorationSize), scaledPixmap);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

std::vector<TextEmoticonsCore::RunningAnimatedImage>::iterator EmoticonItemDelegate::findRunningAnimatedImage(const QModelIndex &index) const
{
    auto matchesIndex = [&](const TextEmoticonsCore::RunningAnimatedImage &rai) {
        return rai.index == index;
    };
    return std::find_if(mRunningAnimatedImages.begin(), mRunningAnimatedImages.end(), matchesIndex);
}

void EmoticonItemDelegate::removeRunningAnimatedImage(const QModelIndex &index) const
{
    auto it = findRunningAnimatedImage(index);
    if (it != mRunningAnimatedImages.end()) {
        mRunningAnimatedImages.erase(it);
    }
}
