/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDebug>
#include <QUrl>
#include <QWizard>
class GenerateVideoMessagePage;
class CreateVideoMessagePage;
class CreateVideoMessageWidget;
class ShowVideoWidget;
class QLineEdit;

class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateVideoMessageWizard : public QWizard
{
    Q_OBJECT
public:
    struct CreateVideoMessageInfo {
        QString mDescription;
        QString mFileName;
        QUrl mFilePath;
        Q_REQUIRED_RESULT bool isValid() const;
    };

    explicit CreateVideoMessageWizard(QWidget *parent = nullptr);
    ~CreateVideoMessageWizard() override;

    Q_REQUIRED_RESULT CreateVideoMessageInfo videoMessageInfo() const;

private:
    void readConfig();
    void writeConfig();
    void slotCurrentIdChanged(int id);
    enum CreateVideoMessageEnum {
        CreateVideo,
        CreateMessage,
    };
    CreateVideoMessagePage *const mCreateVideoMessagePage;
    GenerateVideoMessagePage *const mCreateMessagePage;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateVideoMessagePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit CreateVideoMessagePage(QWidget *parent = nullptr);
    ~CreateVideoMessagePage() override;

    Q_REQUIRED_RESULT bool validatePage() override;

    Q_REQUIRED_RESULT QUrl fileNamePath() const;

    Q_REQUIRED_RESULT bool isComplete() const override;

private:
    CreateVideoMessageWidget *const mCreateVideoMessageWidget;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT GenerateVideoMessagePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit GenerateVideoMessagePage(QWidget *parent = nullptr);
    ~GenerateVideoMessagePage() override;

    Q_REQUIRED_RESULT QString fileName() const;
    Q_REQUIRED_RESULT QString description() const;
    void setFileNamePath(const QUrl &url);
    Q_REQUIRED_RESULT QUrl fileNamePath() const;

private:
    ShowVideoWidget *const mShowVideoWidget;
    QLineEdit *const mFileName;
    QLineEdit *const mDescription;
};

LIBRUQOLAWIDGETS_TESTS_EXPORT QDebug operator<<(QDebug d, const CreateVideoMessageWizard::CreateVideoMessageInfo &t);
