/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
  code based on kdenlive
*/

#include "needupdateparsehtmljob.h"

NeedUpdateParseHtmlJob::NeedUpdateParseHtmlJob(QObject *parent)
    : QObject{parent}
{
}

NeedUpdateParseHtmlJob::~NeedUpdateParseHtmlJob() = default;

bool NeedUpdateParseHtmlJob::canStart() const
{
    // TODO
    return false;
}
