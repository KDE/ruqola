/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "needupdateparsehtmljob.h"
#include "ruqolawidgets_debug.h"
#include <QUrl>
#include <kio/transferjob.h>

NeedUpdateParseHtmlJob::NeedUpdateParseHtmlJob(QObject *parent)
    : QObject{parent}
{
}

NeedUpdateParseHtmlJob::~NeedUpdateParseHtmlJob() = default;

bool NeedUpdateParseHtmlJob::canStart() const
{
    return !mUrl.isEmpty();
}

QUrl NeedUpdateParseHtmlJob::url() const
{
    return mUrl;
}

void NeedUpdateParseHtmlJob::setUrl(const QUrl &newUrl)
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
    KIO::TransferJob *tjob = KIO::get(mUrl, KIO::Reload);
    connect(tjob, &KIO::TransferJob::data, this, &NeedUpdateParseHtmlJob::slotHttpDataFile);
}

void NeedUpdateParseHtmlJob::slotHttpDataFile(KIO::Job *job, const QByteArray &data)
{
    Q_UNUSED(job);
    if (data.isEmpty()) {
        Q_EMIT downLoadDone(mData);
        deleteLater();
    } else {
        mData.append(QString::fromUtf8(data));
    }
}

#include "moc_needupdateparsehtmljob.cpp"
