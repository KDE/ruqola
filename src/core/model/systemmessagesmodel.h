/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    Q_REQUIRED_RESULT QStringList systemMessagesSelected() const;
    void setMessagesSystem(const QStringList &lst);

private:
    void fillModel();
    void createItem(const QString &displayStr, const QString &systemStr);
};

