/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWizardPage>
class KUrlRequester;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ImportDataSelectAccountPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ImportDataSelectAccountPage(QWidget *parent = nullptr);
    ~ImportDataSelectAccountPage() override;

    [[nodiscard]] QUrl zipFileUrl() const;

    [[nodiscard]] bool validatePage() override;

    [[nodiscard]] bool isComplete() const override;

private:
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool verifySelectedUrl() const;
    KUrlRequester *const mUrlRequester;
};
