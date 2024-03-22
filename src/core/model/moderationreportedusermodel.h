/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "custombasemodel.h"
#include "libruqolacore_export.h"
#include "moderation/moderationreporteduserinfos.h"

class LIBRUQOLACORE_EXPORT ModerationReportedUserModel : public CustomBaseModel
{
    Q_OBJECT
public:
    enum ModerationReportedUserRoles {
        ReportDate,
        ReportDateDisplay,
        Reports,
        LastColumn = Reports,
    };
    Q_ENUM(ModerationReportedUserRoles)

    explicit ModerationReportedUserModel(QObject *parent = nullptr);
    ~ModerationReportedUserModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] int total() const override;
    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;
    [[nodiscard]] QList<int> hideColumns() const override;

    void removeElement(const QString &identifier) override;

    [[nodiscard]] const ModerationReportedUserInfos &moderationInfos() const;
    void setModerationInfos(const ModerationReportedUserInfos &userInfos);

private:
    LIBRUQOLACORE_NO_EXPORT void checkFullList() override;
    LIBRUQOLACORE_NO_EXPORT void clear();
    ModerationReportedUserInfos mModerationInfos;
};
