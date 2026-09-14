/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "previewcommandmodel.h"
#include <KIO/TransferJob>
#include <QPixmap>
#include <QUrl>

#include <memory>
#include <utility>

PreviewCommandModel::PreviewCommandModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

PreviewCommandModel::~PreviewCommandModel()
{
    killPendingJobs();
}

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

void PreviewCommandModel::setPreviewCommands(QList<PreviewCommand> newPreviewCommands)
{
    killPendingJobs();
    beginResetModel();
    mMapUrlToImage.clear();
    mPreviewCommands = std::move(newPreviewCommands);
    endResetModel();
    fetchImages();
}

void PreviewCommandModel::killPendingJobs()
{
    for (const auto &job : std::as_const(mPendingJobs)) {
        if (job) {
            // Quietly: result() is not emitted, so the job's lambda can't touch the new list.
            job->kill(KJob::Quietly);
        }
    }
    mPendingJobs.clear();
}

void PreviewCommandModel::fetchImage(const PreviewCommand &command, int idx)
{
    const QString url = command.value();
    // One buffer per job: two items can point to the same url.
    auto buffer = std::make_shared<QByteArray>();
    KIO::TransferJob *transferJob = KIO::get(QUrl(url), KIO::NoReload);
    mPendingJobs.append(transferJob);
    connect(transferJob, &KIO::TransferJob::data, this, [buffer](KIO::Job *, const QByteArray &data) {
        buffer->append(data);
    });
    connect(transferJob, &KIO::TransferJob::result, this, [this, buffer, url, idx](KJob *kjob) {
        mPendingJobs.removeOne(kjob);
        if (kjob->error()) {
            return;
        }
        QPixmap image;
        if (image.loadFromData(*buffer)) {
            mMapUrlToImage.insert(url, image);
            const QModelIndex index = createIndex(idx, 0);
            Q_EMIT dataChanged(index, index, {static_cast<int>(PreviewCommandRoles::Image)});
        }
    });
}

void PreviewCommandModel::fetchImages()
{
    for (int i = 0, total = mPreviewCommands.count(); i < total; ++i) {
        fetchImage(mPreviewCommands.at(i), i);
    }
}

void PreviewCommandModel::clear()
{
    killPendingJobs();
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
        if (const auto it = mMapUrlToImage.constFind(commandPreviewInfo.value()); it != mMapUrlToImage.cend()) {
            return it.value();
        }
        return {};
    }
    return {};
}

#include "moc_previewcommandmodel.cpp"
