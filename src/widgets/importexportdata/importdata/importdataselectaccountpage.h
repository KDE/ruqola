/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT QUrl zipFileUrl() const;

    Q_REQUIRED_RESULT bool validatePage() override;

    Q_REQUIRED_RESULT bool isComplete() const override;

private:
    KUrlRequester *const mUrlRequester;
};
