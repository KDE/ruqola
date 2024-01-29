/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "importexportdata/importexportutils.h"
#include "libruqolawidgets_private_export.h"

#include <QThread>
class KZip;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ExportAccountJob : public QThread
{
    Q_OBJECT
public:
    explicit ExportAccountJob(const QString &fileName, QObject *parent = nullptr);
    ~ExportAccountJob() override;

    [[nodiscard]] QVector<ImportExportUtils::AccountImportExportInfo> listAccounts() const;
    void setListAccounts(const QVector<ImportExportUtils::AccountImportExportInfo> &newListAccounts);

    [[nodiscard]] bool canStart() const;

protected:
    void run() override;

Q_SIGNALS:
    void exportDone();
    void exportFailed(const QString &errorMsg);
    void exportInfo(const QString &msg);

    void exportCacheData(const ImportExportUtils::AccountImportExportInfo &info);
    void exportLogsData(const ImportExportUtils::AccountImportExportInfo &info);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void storeDirectory(const QString &subDirectory, const QString &subfolderPath);
    LIBRUQOLAWIDGETS_NO_EXPORT void exportAccount(const ImportExportUtils::AccountImportExportInfo &info);
    LIBRUQOLAWIDGETS_NO_EXPORT void exportConfig(const ImportExportUtils::AccountImportExportInfo &info);
    LIBRUQOLAWIDGETS_NO_EXPORT void exportCache(const ImportExportUtils::AccountImportExportInfo &info);
    LIBRUQOLAWIDGETS_NO_EXPORT void exportLogs(const ImportExportUtils::AccountImportExportInfo &info);
    LIBRUQOLAWIDGETS_NO_EXPORT void exportDatabase(const ImportExportUtils::AccountImportExportInfo &info);
    LIBRUQOLAWIDGETS_NO_EXPORT void finishExportAccount();
    LIBRUQOLAWIDGETS_NO_EXPORT void exportAccount();
    QVector<ImportExportUtils::AccountImportExportInfo> mListAccounts;
    int mAccountIndex = 0;
    QStringList mAccountNames;
    KZip *const mArchive;
};
