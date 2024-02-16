/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "teams/teamcompleter.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_EXPORT TeamCompleterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum TeamCompleterRoles {
        TeamName = Qt::UserRole + 1,
        TeamId,
        TeamIcon,
    };
    Q_ENUM(TeamCompleterRoles)

    explicit TeamCompleterModel(QObject *parent = nullptr);
    ~TeamCompleterModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clear();
    void insertTeams(const QList<TeamCompleter> &rooms);

private:
    Q_DISABLE_COPY(TeamCompleterModel)
    QList<TeamCompleter> mTeams;
};
