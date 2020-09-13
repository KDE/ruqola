/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef NOTIFICATIONDESKTOPDURATIONPREFERENCEMODEL_H
#define NOTIFICATIONDESKTOPDURATIONPREFERENCEMODEL_H

#include <QAbstractListModel>
#include "libruqolacore_export.h"

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
        NotificationPreference
    };
    Q_ENUM(NotificationPreferenceRoles)

    explicit NotificationDesktopDurationPreferenceModel(QObject *parent = nullptr);
    ~NotificationDesktopDurationPreferenceModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

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

#endif // NOTIFICATIONDESKTOPDURATIONPREFERENCEMODEL_H
