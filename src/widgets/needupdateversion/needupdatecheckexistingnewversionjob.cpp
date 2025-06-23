/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "needupdatecheckexistingnewversionjob.h"
using namespace Qt::Literals::StringLiterals;

#include "needupdateparsehtmljob.h"
#include "needupdateparsehtmlutil.h"
#include "ruqolawidgets_debug.h"

NeedUpdateCheckExistingNewVersionJob::NeedUpdateCheckExistingNewVersionJob(QObject *parent)
    : QObject{parent}
{
}

NeedUpdateCheckExistingNewVersionJob::~NeedUpdateCheckExistingNewVersionJob() = default;

void NeedUpdateCheckExistingNewVersionJob::start()
{
    if (!canStart()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start NeedUpdateCheckExistingNewVersionJob";
        Q_EMIT foundNewVersion(false);
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

    qCDebug(RUQOLAWIDGETS_LOG) << " currentCompiledDate " << mCompileDate;

    const QDate dateFromUrl = QDate::fromString(compileDateStr, u"yyyy-MM-dd"_s);
    qCDebug(RUQOLAWIDGETS_LOG) << " dateFromUrl " << dateFromUrl << " original " << compileDateStr;

    if (dateFromUrl > mCompileDate) {
        Q_EMIT foundNewVersion(true);
    } else {
        Q_EMIT foundNewVersion(false);
    }
    deleteLater();
}

QUrl NeedUpdateCheckExistingNewVersionJob::url() const
{
    return mUrl;
}

void NeedUpdateCheckExistingNewVersionJob::setUrl(const QUrl &newUrl)
{
    mUrl = newUrl;
}

bool NeedUpdateCheckExistingNewVersionJob::canStart() const
{
    return !mUrl.isEmpty() && mCompileDate.isValid();
}

QDate NeedUpdateCheckExistingNewVersionJob::compileDate() const
{
    return mCompileDate;
}

void NeedUpdateCheckExistingNewVersionJob::setCompileDate(const QDate &newCompileDate)
{
    mCompileDate = newCompileDate;
}

#include "moc_needupdatecheckexistingnewversionjob.cpp"
