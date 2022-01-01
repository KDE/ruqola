/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "changetemporarystatus.h"
#include <QTimer>
// TODO use QTimer to change status
ChangeTemporaryStatus::ChangeTemporaryStatus(QObject *parent)
    : QObject(parent)
    , mTimer(new QTimer(this))
{
    initialize();
}

void ChangeTemporaryStatus::initialize()
{
}
