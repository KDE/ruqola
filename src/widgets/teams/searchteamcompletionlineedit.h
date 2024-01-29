/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "common/completionlineedit.h"
#include "libruqolawidgets_private_export.h"
class TeamCompleterModel;
class QTimer;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SearchTeamCompletionLineEdit : public CompletionLineEdit
{
    Q_OBJECT
public:
    explicit SearchTeamCompletionLineEdit(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SearchTeamCompletionLineEdit() override;

    [[nodiscard]] const QString &teamId() const;

Q_SIGNALS:
    void teamIdChanged(bool isEmpty);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchTimerFired();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchTextEdited();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTextChanged(const QString &text);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotComplete(const QModelIndex &index);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotTeamAutoCompleteDone(const QJsonObject &obj);
    QString mTeamId;
    TeamCompleterModel *const mTeamCompleterModel;
    QTimer *const mSearchTimer;
    RocketChatAccount *const mRocketChatAccount;
};
