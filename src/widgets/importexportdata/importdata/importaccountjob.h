/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QObject>

class KZip;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ImportAccountJob : public QObject
{
    Q_OBJECT
public:
    explicit ImportAccountJob(const QString &fileName, QObject *parent = nullptr);
    ~ImportAccountJob() override;

    void start();
    Q_REQUIRED_RESULT bool canStart() const;

    Q_REQUIRED_RESULT QUrl zipPath() const;
    void setZipPath(const QUrl &newZipPath);

private:
    KZip *const mArchive;
};
