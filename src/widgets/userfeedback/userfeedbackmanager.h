/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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

