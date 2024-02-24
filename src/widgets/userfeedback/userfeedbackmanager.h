/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
namespace KUserFeedback
{
class Provider;
}
class UserFeedBackManager : public QObject
{
    Q_OBJECT
public:
    ~UserFeedBackManager() override;
    static UserFeedBackManager *self();

    [[nodiscard]] KUserFeedback::Provider *userFeedbackProvider() const;

private:
    explicit UserFeedBackManager(QObject *parent = nullptr);
    KUserFeedback::Provider *const mUserFeedbackProvider;
};
