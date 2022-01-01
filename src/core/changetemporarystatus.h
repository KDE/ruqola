/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>
class QTimer;
class ChangeTemporaryStatus : public QObject
{
    Q_OBJECT
public:
    explicit ChangeTemporaryStatus(QObject *parent = nullptr);
    ~ChangeTemporaryStatus() override = default;

Q_SIGNALS:
    void changeStatusAway(bool away);

private:
    Q_DISABLE_COPY(ChangeTemporaryStatus)
    void initialize();
    QTimer *const mTimer;
};

