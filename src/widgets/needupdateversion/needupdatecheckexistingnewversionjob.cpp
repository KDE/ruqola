/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "needupdatecheckexistingnewversionjob.h"
#include "needupdateparsehtmljob.h"
#include "needupdateparsehtmlutil.h"
#include "ruqolawidgets_debug.h"

#include <QDate>

NeedUpdateCheckExistingNewVersionJob::NeedUpdateCheckExistingNewVersionJob(QObject *parent)
    : QObject{parent}
{
}

NeedUpdateCheckExistingNewVersionJob::~NeedUpdateCheckExistingNewVersionJob() = default;

void NeedUpdateCheckExistingNewVersionJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start NeedUpdateCheckExistingNewVersionJob";
        deleteLater();
        return;
    }
    auto job = new NeedUpdateParseHtmlJob(this);
    job->setUrl(mUrl);
    connect(job, &NeedUpdateParseHtmlJob::downLoadDone, this, &NeedUpdateCheckExistingNewVersionJob::slotDownloadDone);
    job->start();
}

void NeedUpdateCheckExistingNewVersionJob::slotDownloadDone(const QString &str)
{
    const QString compileDateStr = NeedUpdateParseHtmlUtil::extractDate(str);
    if (compileDateStr.isEmpty()) {
        Q_EMIT foundNewVersion(false);
        deleteLater();
        return;
    }

    const QDate currentCompiledDate = QDate::fromString(mCompileDate, QStringLiteral("MMM dd yyyy"));
    qCDebug(RUQOLAWIDGETS_LOG) << " currentCompiledDate " << currentCompiledDate << " original " << mCompileDate;

    const QDate dateFromUrl = QDate::fromString(compileDateStr, QStringLiteral("yyyy-MM-dd"));
    qCDebug(RUQOLAWIDGETS_LOG) << " dateFromUrl " << dateFromUrl << " original " << compileDateStr;

    if (dateFromUrl > currentCompiledDate) {
        Q_EMIT foundNewVersion(true);
    } else {
        Q_EMIT foundNewVersion(false);
    }
    deleteLater();
}

QString NeedUpdateCheckExistingNewVersionJob::url() const
{
    return mUrl;
}

void NeedUpdateCheckExistingNewVersionJob::setUrl(const QString &newUrl)
{
    mUrl = newUrl;
}

bool NeedUpdateCheckExistingNewVersionJob::canStart() const
{
    return !mUrl.isEmpty() && !mCompileDate.isEmpty();
}

QString NeedUpdateCheckExistingNewVersionJob::compileDate() const
{
    return mCompileDate;
}

void NeedUpdateCheckExistingNewVersionJob::setCompileDate(const QString &newCompileDate)
{
    mCompileDate = newCompileDate;
}

#include "moc_needupdatecheckexistingnewversionjob.cpp"
