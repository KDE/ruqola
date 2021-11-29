/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QString>
class QWidget;
class QUrl;
namespace DelegateUtil
{
Q_REQUIRED_RESULT QString querySaveFileName(QWidget *parent, const QString &title, const QUrl &fileToSave);
void saveFile(QWidget *parent, const QString &filePath, const QString &title);
}

