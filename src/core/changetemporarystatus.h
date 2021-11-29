/*
   SPDX-FileCopyrightText: 2017-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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

