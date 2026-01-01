/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
        Permissions,
    };
    Q_ENUM(CommandsRoles)

    explicit CommandsModel(QObject *parent = nullptr);
    ~CommandsModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] Commands commands() const;

    void setCommands(const Commands &emoticons);

    [[nodiscard]] bool commandHasPreview(const QString &commandName) const;

private:
    LIBRUQOLACORE_NO_EXPORT void clear();
    Commands mCommands;
};
