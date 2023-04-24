/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "textemoticonswidgets_private_export.h"
#include <QStyledItemDelegate>
#include <TextEmoticonsCore/RunningAnimatedImage>
#include <vector>
namespace TextEmoticonsWidgets
{
class TEXTEMOTICONSWIDGETS_TESTS_EXPORT EmoticonItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit EmoticonItemDelegate(QObject *parent = nullptr);
    ~EmoticonItemDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    std::vector<TextEmoticonsCore::RunningAnimatedImage>::iterator findRunningAnimatedImage(const QModelIndex &index) const;
    void removeRunningAnimatedImage(const QModelIndex &index) const;
    mutable std::vector<TextEmoticonsCore::RunningAnimatedImage> mRunningAnimatedImages; // not a hash or map, since QPersistentModelIndex changes value
};
}
