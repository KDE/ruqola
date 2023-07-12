/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT QVector<ImportExportUtils::AccountImportExportInfo> listAccounts() const;
    void setListAccounts(const QVector<ImportExportUtils::AccountImportExportInfo> &newListAccounts);

    Q_REQUIRED_RESULT bool canStart() const;

protected:
    void run() override;

Q_SIGNALS:
    void exportDone();
    void exportFailed(const QString &errorMsg);
    void exportInfo(const QString &msg);

    void exportCacheData(const ImportExportUtils::AccountImportExportInfo &info);
    void exportLogsData(const ImportExportUtils::AccountImportExportInfo &info);

private:
    void storeDirectory(const QString &subDirectory, const QString &subfolderPath);
    void exportAccount(const ImportExportUtils::AccountImportExportInfo &info);
    void exportConfig(const ImportExportUtils::AccountImportExportInfo &info);
    void exportCache(const ImportExportUtils::AccountImportExportInfo &info);
    void exportLogs(const ImportExportUtils::AccountImportExportInfo &info);
    void exportDatabase(const ImportExportUtils::AccountImportExportInfo &info);
    void finishExportAccount();
    void exportAccount();
    QVector<ImportExportUtils::AccountImportExportInfo> mListAccounts;
    int mAccountIndex = 0;
    QStringList mAccountNames;
    KZip *const mArchive;
};
