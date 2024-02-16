/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QAbstractListModel>

struct NotificationDesktopSoundPreferenceInfo {
    QString displayText;
    QString preference;
};

class LIBRUQOLACORE_EXPORT NotificationDesktopSoundPreferenceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NotificationPreferenceRoles {
        NotificationPreferenceI18n = Qt::UserRole + 1,
        NotificationPreference,
    };
    Q_ENUM(NotificationPreferenceRoles)

    explicit NotificationDesktopSoundPreferenceModel(QObject *parent = nullptr);
    ~NotificationDesktopSoundPreferenceModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    [[nodiscard]] int setCurrentNotificationPreference(const QString &preference);

    [[nodiscard]] QString currentPreference(int index) const;

Q_SIGNALS:
    void currentNotificationPreferenceChanged();

private:
    LIBRUQOLACORE_NO_EXPORT void fillModel();
    QList<NotificationDesktopSoundPreferenceInfo> mNotificationDestktopSoundPreferenceList;
    int mCurrentPreference = 0;
};
