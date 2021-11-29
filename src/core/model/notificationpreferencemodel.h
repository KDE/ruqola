/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"

#include <QAbstractListModel>

struct NotificationPreferenceInfo {
    QString displayText;
    QString preference;
};

class LIBRUQOLACORE_EXPORT NotificationPreferenceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NotificationPreferenceRoles {
        NotificationPreferenceI18n = Qt::UserRole + 1,
        NotificationPreference,
    };
    Q_ENUM(NotificationPreferenceRoles)

    explicit NotificationPreferenceModel(QObject *parent = nullptr);
    ~NotificationPreferenceModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_REQUIRED_RESULT int setCurrentNotificationPreference(const QString &preference);

    Q_REQUIRED_RESULT QString currentPreference(int index) const;

Q_SIGNALS:
    void currentNotificationPreferenceChanged();

private:
    Q_DISABLE_COPY(NotificationPreferenceModel)
    void fillModel();
    QVector<NotificationPreferenceInfo> mNotificationPreferenceList;
    int mCurrentPreference = 0;
};

