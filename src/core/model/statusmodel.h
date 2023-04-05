/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "user.h"

#include <QAbstractListModel>
#include <QIcon>
class CustomUserStatus;

class LIBRUQOLACORE_EXPORT StatusModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum StatusRoles {
        StatusI18n = Qt::UserRole + 1,
        Status,
        Icon,
        Order,
    };
    Q_ENUM(StatusRoles)

    struct StatusInfo {
        User::PresenceStatus status;
        QString statusStr;
    };

    explicit StatusModel(QObject *parent = nullptr);
    ~StatusModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_REQUIRED_RESULT User::PresenceStatus status(int index) const;

    Q_REQUIRED_RESULT int currentStatus() const;

    void setCurrentPresenceStatus(User::PresenceStatus status);
    Q_REQUIRED_RESULT User::PresenceStatus currentUserStatus() const;

    Q_REQUIRED_RESULT StatusInfo currentStatusInfo() const;

    Q_REQUIRED_RESULT QString customText() const;
    void setCustomText(const QString &customText);

    void updateCustomStatus(const QVector<CustomUserStatus> &customUserStatuses);

Q_SIGNALS:
    void currentStatusChanged();

private:
    struct DisplayStatusInfo {
        QString displayText;
        QString statusStr;
        QIcon icon;
        User::PresenceStatus status;
        int order = 0;
    };
    Q_REQUIRED_RESULT LIBRUQOLACORE_NO_EXPORT QString textFromPresenceStatus(User::PresenceStatus status) const;
    Q_REQUIRED_RESULT LIBRUQOLACORE_NO_EXPORT QIcon iconFromPresenceStatus(User::PresenceStatus status) const;
    Q_REQUIRED_RESULT LIBRUQOLACORE_NO_EXPORT StatusModel::DisplayStatusInfo createStatusInfo(User::PresenceStatus status, int order);
    Q_DISABLE_COPY(StatusModel)
    LIBRUQOLACORE_NO_EXPORT void fillModel();
    QVector<DisplayStatusInfo> mStatusList;
    QString mCustomText;
    int mCurrentStatus = 0;
};
