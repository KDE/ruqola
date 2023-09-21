/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "custombasemodel.h"
#include "libruqolacore_export.h"
#include "moderation/moderationinfos.h"

class LIBRUQOLACORE_EXPORT ModerationModel : public CustomBaseModel
{
    Q_OBJECT
public:
    enum ModerationInfoRoles {
        Count,
        Name,
        UserName,
        Message,
        UserDeleted,
        UserId,
        MessageId,
        LastColumn = MessageId,
    };
    Q_ENUM(ModerationInfoRoles)

    explicit ModerationModel(QObject *parent = nullptr);
    ~ModerationModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;
    Q_REQUIRED_RESULT QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Q_REQUIRED_RESULT int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_REQUIRED_RESULT int total() const override;
    void parseElements(const QJsonObject &obj) override;
    void addMoreElements(const QJsonObject &obj) override;
    Q_REQUIRED_RESULT QList<int> hideColumns() const override;

    void removeElement(const QString &identifier) override;

    Q_REQUIRED_RESULT const ModerationInfos &moderationInfos() const;
    void setModerationInfos(const ModerationInfos &newDeviceInfos);

private:
    Q_DISABLE_COPY(ModerationModel)
    LIBRUQOLACORE_NO_EXPORT void checkFullList();
    ModerationInfos mModerationInfos;
};
