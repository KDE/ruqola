/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewcommandmodel.h"
#include <KIO/TransferJob>
#include <QPixmap>
#include <QUrl>

PreviewCommandModel::PreviewCommandModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

PreviewCommandModel::~PreviewCommandModel() = default;

int PreviewCommandModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }

    return mPreviewCommands.count();
}

QList<PreviewCommand> PreviewCommandModel::previewCommands() const
{
    return mPreviewCommands;
}

void PreviewCommandModel::setPreviewCommands(const QList<PreviewCommand> &newPreviewCommands)
{
    clear();
    if (!newPreviewCommands.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, newPreviewCommands.count() - 1);
        mPreviewCommands = newPreviewCommands;
        endInsertRows();
        fetchImages();
    }
}

void PreviewCommandModel::fetchImage(const PreviewCommand &command, int index)
{
    QByteArray imageData;
    const QString url = command.value();
    KIO::TransferJob *job = KIO::get(QUrl(url), KIO::NoReload);
    connect(job, &KIO::TransferJob::data, this, [&imageData](KIO::Job *, const QByteArray &data) {
        imageData.append(data);
    });
    if (job->exec()) {
        QPixmap image;
        if (image.loadFromData(imageData)) {
            mMapUrlToImage.insert(url, image);
            const int row = index;
            if (row != -1) {
                const QModelIndex index = createIndex(row, 0);
                Q_EMIT dataChanged(index, index, {static_cast<int>(PreviewCommandRoles::Image)});
            }
        }
    }
}

void PreviewCommandModel::fetchImages()
{
    for (int i = 0, total = mPreviewCommands.count(); i < total; ++i) {
        fetchImage(mPreviewCommands.at(i), i);
    }
}

void PreviewCommandModel::clear()
{
    if (!mPreviewCommands.isEmpty()) {
        beginResetModel();
        mPreviewCommands.clear();
        endResetModel();
    }
    mMapUrlToImage.clear();
}

QVariant PreviewCommandModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mPreviewCommands.count()) {
        return {};
    }

    const PreviewCommand &commandPreviewInfo = mPreviewCommands.at(index.row());
    if (role == Qt::DisplayRole) {
        return commandPreviewInfo.value();
    } else if (role == static_cast<int>(PreviewCommandRoles::PreviewCommandInfo)) {
        return QVariant::fromValue(commandPreviewInfo);
    } else if (role == static_cast<int>(PreviewCommandRoles::Image)) {
        if (mMapUrlToImage.contains(commandPreviewInfo.value())) {
            return mMapUrlToImage.value(commandPreviewInfo.value());
        }
        return {};
    }
    return {};
}

#include "moc_previewcommandmodel.cpp"
