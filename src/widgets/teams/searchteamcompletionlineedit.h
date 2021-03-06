/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#pragma once

#include "common/completionlineedit.h"
#include "libruqolawidgets_private_export.h"
class TeamCompleterModel;
class QTimer;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchTeamCompletionLineEdit : public CompletionLineEdit
{
    Q_OBJECT
public:
    explicit SearchTeamCompletionLineEdit(QWidget *parent = nullptr);
    ~SearchTeamCompletionLineEdit() override;

    const QString &teamId() const;

private:
    void slotSearchTimerFired();
    void slotSearchTextEdited();
    void slotTextChanged(const QString &text);
    void slotComplete(const QModelIndex &index);
    void slotTeamAutoCompleteDone(const QJsonObject &obj);
    QString mTeamId;
    TeamCompleterModel *const mTeamCompleterModel;
    QTimer *const mSearchTimer;
};
