/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"

#include <QObject>

class LIBRUQOLAWIDGETS_TESTS_EXPORT ExportAccountJob : public QObject
{
    Q_OBJECT
public:
    explicit ExportAccountJob(QObject *parent = nullptr);
    ~ExportAccountJob() override;

    void start();

    Q_REQUIRED_RESULT QStringList listAccounts() const;
    void setListAccounts(const QStringList &newListAccounts);

    Q_REQUIRED_RESULT bool canStart() const;

Q_SIGNALS:
    void exportDone();

private:
    QStringList mListAccounts;
};
