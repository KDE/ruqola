/*
   SPDX-FileCopyrightText: 2019-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"

#include <QAbstractListModel>

struct NotificationPreferenceInfo {
    QString displayText;
    QByteArray preference;
};

class LIBRUQOLACORE_EXPORT NotificationPreferenceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NotificationPreferenceRoles {
        NotificationPreferenceI18nRole = Qt::UserRole + 1,
        NotificationPreferenceRole,
    };
    Q_ENUM(NotificationPreferenceRoles)

    explicit NotificationPreferenceModel(QObject *parent = nullptr);
    ~NotificationPreferenceModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    [[nodiscard]] int setCurrentNotificationPreference(const QByteArray &preference);

    [[nodiscard]] QByteArray currentPreference(int index) const;

Q_SIGNALS:
    void currentNotificationPreferenceChanged();

private:
    LIBRUQOLACORE_NO_EXPORT void fillModel();
    QList<NotificationPreferenceInfo> mNotificationPreferenceList;
    int mCurrentPreference = 0;
};
