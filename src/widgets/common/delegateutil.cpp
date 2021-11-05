/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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

#include "delegateutil.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>

#include <QDir>
#include <QFileDialog>
#include <QMimeDatabase>
#include <QStandardPaths>
#include <QUrl>

QString DelegateUtil::querySaveFileName(QWidget *parent, const QString &title, const QUrl &fileToSave)
{
    const auto dir = QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    const auto info = QFileInfo(fileToSave.path());
    auto fileName = info.fileName();
    if (fileToSave.isLocalFile() && info.exists() && info.suffix().isEmpty()) {
        // guess a proper file suffix if none is given
        [&]() {
            QFile file(info.absoluteFilePath());
            if (!file.open(QIODevice::ReadOnly)) {
                return;
            }
            QMimeDatabase mimeDb;
            const auto mime = mimeDb.mimeTypeForFileNameAndData(fileName, &file);
            if (!mime.isValid()) {
                return;
            }
            const auto suffix = mime.preferredSuffix();
            if (!suffix.isEmpty()) {
                fileName += QLatin1Char('.') + suffix;
            }
        }();
    }
    return QFileDialog::getSaveFileName(parent, title, dir.absoluteFilePath(fileName));
}

void DelegateUtil::saveFile(QWidget *parentWidget, const QString &filePath, const QString &title)
{
    const auto file = DelegateUtil::querySaveFileName(parentWidget, title, QUrl::fromLocalFile(filePath));
    if (!file.isEmpty()) {
        if (QFile::exists(file) && !QFile::remove(file)) { // copy() doesn't overwrite
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to remove : " << file;
        }
        QFile sourceFile(filePath);
        if (!sourceFile.copy(file)) {
            KMessageBox::error(parentWidget, sourceFile.errorString(), i18n("Error saving file"));
        }
    }
}
