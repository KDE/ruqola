/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "needupdatecheckexistingnewversionjob.h"

NeedUpdateCheckExistingNewVersionJob::NeedUpdateCheckExistingNewVersionJob(QObject *parent)
    : QObject{parent}
{
}

NeedUpdateCheckExistingNewVersionJob::~NeedUpdateCheckExistingNewVersionJob() = default;

#include "moc_needupdatecheckexistingnewversionjob.cpp"
