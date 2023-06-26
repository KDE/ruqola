/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QObject>

class KZip;
class KArchiveDirectory;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ImportAccountJob : public QObject
{
    Q_OBJECT
public:
    explicit ImportAccountJob(const QString &fileName, QObject *parent = nullptr);
    ~ImportAccountJob() override;

    void start();

Q_SIGNALS:
    void importFailed(const QString &str);
    void importDone();

private:
    void copyToDirectory(const KArchiveDirectory *subfolderDir, const QString &dest);
    void importAccount(QString accountName);
    Q_REQUIRED_RESULT QString verifyExistingAccount(QString accountName) const;
    KZip *const mArchive;
};
