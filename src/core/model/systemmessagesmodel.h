/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"

#include <QStandardItemModel>

class LIBRUQOLACORE_EXPORT SystemMessagesModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum SystemMessagesRoles {
        SystemMessagesI18n = Qt::UserRole + 1,
        SystemMessages,
    };
    Q_ENUM(SystemMessagesRoles)
    explicit SystemMessagesModel(QObject *parent = nullptr);
    ~SystemMessagesModel() override;
    [[nodiscard]] QStringList systemMessagesSelected() const;
    void setMessagesSystem(const QStringList &lst);

private:
    LIBRUQOLACORE_NO_EXPORT void fillModel();
    LIBRUQOLACORE_NO_EXPORT void createItem(const QString &displayStr, const QString &systemStr);
};
