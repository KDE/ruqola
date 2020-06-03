/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#include "messagedelegatehelperbase.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QUrl>

MessageDelegateHelperBase::~MessageDelegateHelperBase()
{
}

bool MessageDelegateHelperBase::handleMouseEvent(QMouseEvent *mouseEvent, const QRect &attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(mouseEvent)
    Q_UNUSED(attachmentsRect)
    Q_UNUSED(option)
    Q_UNUSED(index)
    return false;
}

QString MessageDelegateHelperBase::querySaveFileName(QWidget *parent, const QString &title, const QUrl &fileToSave)
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
