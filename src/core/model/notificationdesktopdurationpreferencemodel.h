/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT NotificationDesktopDurationPreferenceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    struct NotificationDesktopDurationPreferenceInfo {
        QString displayText;
        QString preference;
    };
    enum NotificationPreferenceRoles {
        NotificationPreferenceI18n = Qt::UserRole + 1,
        NotificationPreference,
    };
    Q_ENUM(NotificationPreferenceRoles)

    explicit NotificationDesktopDurationPreferenceModel(QObject *parent = nullptr);
    ~NotificationDesktopDurationPreferenceModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    [[nodiscard]] int setCurrentNotificationPreference(const QString &preference);

    [[nodiscard]] QString currentPreference(int index) const;

Q_SIGNALS:
    void currentNotificationPreferenceChanged();

private:
    LIBRUQOLACORE_NO_EXPORT void fillModel();
    QList<NotificationDesktopDurationPreferenceInfo> mNotificationDestktopDurationPreferenceList;
    int mCurrentPreference = 0;
};
Q_DECLARE_METATYPE(NotificationDesktopDurationPreferenceModel::NotificationDesktopDurationPreferenceInfo)
Q_DECLARE_TYPEINFO(NotificationDesktopDurationPreferenceModel::NotificationDesktopDurationPreferenceInfo, Q_RELOCATABLE_TYPE);
