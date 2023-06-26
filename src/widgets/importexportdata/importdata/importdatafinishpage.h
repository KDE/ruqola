/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWizardPage>
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ImportDataFinishPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ImportDataFinishPage(QWidget *parent = nullptr);
    ~ImportDataFinishPage() override;

    void setZipFileUrl(const QUrl &url);

private:
    void slotImportDone();
    void slotImportFailed(const QString &msg);

    QLabel *const mInfos;
};
