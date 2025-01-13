/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "needupdateparsehtmljob.h"
#include "ruqolawidgets_debug.h"

NeedUpdateParseHtmlJob::NeedUpdateParseHtmlJob(QObject *parent)
    : QObject{parent}
{
}

NeedUpdateParseHtmlJob::~NeedUpdateParseHtmlJob() = default;

bool NeedUpdateParseHtmlJob::canStart() const
{
    return !mUrl.isEmpty();
}

QString NeedUpdateParseHtmlJob::url() const
{
    return mUrl;
}

void NeedUpdateParseHtmlJob::setUrl(const QString &newUrl)
{
    mUrl = newUrl;
}

void NeedUpdateParseHtmlJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start NeedUpdateParseHtmlJob";
        deleteLater();
        return;
    }
    // TODO
    deleteLater();
}

#include "moc_needupdateparsehtmljob.cpp"
