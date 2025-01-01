/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QThread>

class KZip;
class KArchiveDirectory;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ImportAccountJob : public QThread
{
    Q_OBJECT
public:
    explicit ImportAccountJob(const QString &fileName, QObject *parent = nullptr);
    ~ImportAccountJob() override;

protected:
    void run() override;

Q_SIGNALS:
    void importFailed(const QString &str);
    void importDone();
    void importInfo(const QString &msg);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void copyToDirectory(const KArchiveDirectory *subfolderDir, const QString &dest);
    LIBRUQOLAWIDGETS_NO_EXPORT void importAccount(QString accountName);
    LIBRUQOLAWIDGETS_NO_EXPORT void finishImportAccounts();
    LIBRUQOLAWIDGETS_NO_EXPORT void importAccounts();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString verifyExistingAccount(QString accountName) const;
    QStringList mAccountInfos;
    KZip *const mArchive;
    int mAccountIndex = 0;
};
