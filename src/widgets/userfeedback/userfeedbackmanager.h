/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

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
    explicit UserFeedBackManager(QObject *parent = nullptr);
    ~UserFeedBackManager() override;
    static UserFeedBackManager *self();

    Q_REQUIRED_RESULT KUserFeedback::Provider *userFeedbackProvider() const;

private:
    KUserFeedback::Provider *const mUserFeedbackProvider;
};
