/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "copyfilejob.h"
#include "ruqola_debug.h"
#include <QFile>

CopyFileJob::CopyFileJob(QObject *parent)
    : KJob(parent)
{
}

void CopyFileJob::start()
{
    if (mLocalFile.isEmpty() || mCachedFile.isEmpty()) {
        qCWarning(RUQOLA_LOG) << " localfile or cachedfile not defined";
    } else {
        QFile f(mCachedFile);
        if (!f.copy(mLocalFile)) {
            qCWarning(RUQOLA_LOG) << "Impossible to copy " << mCachedFile << " to " << mLocalFile;
        }
    }
}

void CopyFileJob::setLocalFile(const QString &localFile)
{
    mLocalFile = localFile;
}

void CopyFileJob::setCachedFile(const QString &cachedFile)
{
    mCachedFile = cachedFile;
}
