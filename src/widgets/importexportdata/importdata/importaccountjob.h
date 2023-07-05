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
    void importInfo(const QString &msg);

private:
    void copyToDirectory(const KArchiveDirectory *subfolderDir, const QString &dest);
    void importAccount(QString accountName);
    void finishImportAccounts();
    void importAccounts();
    Q_REQUIRED_RESULT QString verifyExistingAccount(QString accountName) const;
    QStringList mAccountInfos;
    KZip *const mArchive;
    int mAccountIndex = 0;
};
