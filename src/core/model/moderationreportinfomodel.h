/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QAbstractListModel>

#include "libruqolacore_export.h"
#include "moderation/moderationreportinfos.h"
#include <QVector>

class LIBRUQOLACORE_EXPORT ModerationReportInfoModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NotificationHistoryRoles {
        Message = Qt::UserRole + 1,
    };
    Q_ENUM(NotificationHistoryRoles)

    explicit ModerationReportInfoModel(QObject *parent = nullptr);
    ~ModerationReportInfoModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    void clear();

    void insertModerationReportInfo(const ModerationReportInfos &infos);

private:
    ModerationReportInfos mModerationReportInfos;
};
