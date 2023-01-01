/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QAbstractListModel>

struct NotificationDesktopDurationPreferenceInfo {
    QString displayText;
    QString preference;
};

class LIBRUQOLACORE_EXPORT NotificationDesktopDurationPreferenceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NotificationPreferenceRoles {
        NotificationPreferenceI18n = Qt::UserRole + 1,
        NotificationPreference,
    };
    Q_ENUM(NotificationPreferenceRoles)

    explicit NotificationDesktopDurationPreferenceModel(QObject *parent = nullptr);
    ~NotificationDesktopDurationPreferenceModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_REQUIRED_RESULT int setCurrentNotificationPreference(const QString &preference);

    Q_REQUIRED_RESULT QString currentPreference(int index) const;

Q_SIGNALS:
    void currentNotificationPreferenceChanged();

private:
    Q_DISABLE_COPY(NotificationDesktopDurationPreferenceModel)
    void fillModel();
    QVector<NotificationDesktopDurationPreferenceInfo> mNotificationDestktopDurationPreferenceList;
    int mCurrentPreference = 0;
};
