/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "custombasemodel.h"
#include "libruqolacore_export.h"
#include "moderation/moderationreportedmessageinfos.h"

class LIBRUQOLACORE_EXPORT ModerationReportedMessageModel : public CustomBaseModel
{
    Q_OBJECT
public:
    enum ModerationReportedMessageRoles {
        Name,
        UserName,
        Message,
        RoomName,
        ReportDate,
        ReportDateDisplay,
        Reports,
        UserDeleted,
        UserId,
        MessageId,
        LastColumn = MessageId,
    };
    Q_ENUM(ModerationReportedMessageRoles)

    explicit ModerationReportedMessageModel(QObject *parent = nullptr);
    ~ModerationReportedMessageModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] int total() const override;
    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;
    [[nodiscard]] QList<int> hideColumns() const override;

    void removeElement(const QByteArray &identifier) override;

    [[nodiscard]] const ModerationReportedMessageInfos &moderationInfos() const;
    void setModerationInfos(const ModerationReportedMessageInfos &newDeviceInfos);

private:
    LIBRUQOLACORE_NO_EXPORT void checkFullList() override;
    LIBRUQOLACORE_NO_EXPORT void clear();
    ModerationReportedMessageInfos mModerationInfos;
};
