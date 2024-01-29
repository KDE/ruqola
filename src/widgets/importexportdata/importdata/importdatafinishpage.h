/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWizardPage>
class QLabel;
class KMessageWidget;
class QPlainTextEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ImportDataFinishPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ImportDataFinishPage(QWidget *parent = nullptr);
    ~ImportDataFinishPage() override;

    void setZipFileUrl(const QUrl &url);

    [[nodiscard]] bool isComplete() const override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotImportDone();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotImportFailed(const QString &msg);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotImportInfo(const QString &msg);

    QLabel *const mInfos;
    KMessageWidget *const mMessageWidget;
    QPlainTextEdit *const mDetails;
    bool mImportDone = false;
};
