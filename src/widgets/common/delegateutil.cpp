/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "delegateutil.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KRecentDirs>

#include <QFileDialog>
#include <QMimeDatabase>
#include <QStandardPaths>
#include <QUrl>

#include <KFileWidget>

QString DelegateUtil::querySaveFileName(QWidget *parent, const QString &title, const QUrl &fileToSave)
{
    QString fileClass;
    const QUrl startUrl = KFileWidget::getStartUrl(QUrl(QStringLiteral("kfiledialog:///saveattachment")), fileClass);
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
    QUrl localUrl;
    localUrl.setPath(startUrl.path() + QLatin1Char('/') + fileName);
    const QString fileStr = QFileDialog::getSaveFileName(parent, title, localUrl.toString());

    if (!fileClass.isEmpty() && !fileStr.isEmpty()) {
        KRecentDirs::add(fileClass, fileStr);
    }
    return fileStr;
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
