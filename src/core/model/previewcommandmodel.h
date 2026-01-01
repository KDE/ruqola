/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "commands/previewcommand.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT PreviewCommandModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum class PreviewCommandRoles : uint16_t {
        PreviewCommandInfo = Qt::UserRole + 1,
        Image,
    };
    explicit PreviewCommandModel(QObject *parent = nullptr);
    ~PreviewCommandModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QList<PreviewCommand> previewCommands() const;
    void setPreviewCommands(const QList<PreviewCommand> &newPermissions);
    void clear();

private:
    LIBRUQOLACORE_NO_EXPORT void fetchImage(const PreviewCommand &command, int index);
    LIBRUQOLACORE_NO_EXPORT void fetchImages();
    QList<PreviewCommand> mPreviewCommands;
    QMap<QString, QPixmap> mMapUrlToImage;
};
