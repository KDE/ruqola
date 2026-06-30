/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "readreceipts/readreceipts.h"
#include <QList>
#include <QStandardItemModel>
class RoleInfo;
class LIBRUQOLACORE_EXPORT ReadReceiptsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum class ReadReceiptsInfo : uint16_t {
        UserId = Qt::UserRole + 1,
        UserName,
        Name,
        TimeStamp,
    };
    Q_ENUM(ReadReceiptsInfo)

    explicit ReadReceiptsModel(QObject *parent = nullptr);
    ~ReadReceiptsModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    [[nodiscard]] ReadReceipts readReceipts() const;
    void setReadReceipts(const ReadReceipts &newReadReceipts);

private:
    ReadReceipts mReadReceipts;
};
