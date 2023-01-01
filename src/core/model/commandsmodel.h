/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "commands/commands.h"
#include "libruqolacore_export.h"
#include <QAbstractListModel>

// Model showing all emojis
class LIBRUQOLACORE_EXPORT CommandsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CommandsRoles {
        CommandName = Qt::UserRole + 1,
        CompleterName, // keep value in sync with InputCompleterModel
        Description,
        Parameters,
        TranslatedDescription,
        TranslatedParams,
    };
    Q_ENUM(CommandsRoles)

    explicit CommandsModel(QObject *parent = nullptr);
    ~CommandsModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    Q_REQUIRED_RESULT Commands commands() const;

    void setCommands(const Commands &emoticons);

private:
    Q_DISABLE_COPY(CommandsModel)
    Commands mCommands;
};
