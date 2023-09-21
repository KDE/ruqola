/*
   SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
        NotificationPreferenceI18nRole = Qt::UserRole + 1,
        NotificationPreferenceRole,
    };
    Q_ENUM(NotificationPreferenceRoles)

    explicit NotificationPreferenceModel(QObject *parent = nullptr);
    ~NotificationPreferenceModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    [[nodiscard]] int setCurrentNotificationPreference(const QString &preference);

    [[nodiscard]] QString currentPreference(int index) const;

Q_SIGNALS:
    void currentNotificationPreferenceChanged();

private:
    Q_DISABLE_COPY(NotificationPreferenceModel)
    LIBRUQOLACORE_NO_EXPORT void fillModel();
    QVector<NotificationPreferenceInfo> mNotificationPreferenceList;
    int mCurrentPreference = 0;
};
