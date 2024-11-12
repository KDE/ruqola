/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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
class RocketChatAccount;

class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateVideoMessageWizard : public QWizard
{
    Q_OBJECT
public:
    struct LIBRUQOLAWIDGETS_TESTS_EXPORT CreateVideoMessageInfo {
        QString mDescription;
        QString mFileName;
        QUrl mFileUrl;
        [[nodiscard]] bool isValid() const;
    };

    explicit CreateVideoMessageWizard(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CreateVideoMessageWizard() override;

    [[nodiscard]] CreateVideoMessageInfo videoMessageInfo() const;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCurrentIdChanged(int id);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAccepted();
    enum CreateVideoMessageEnum : uint8_t {
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

    [[nodiscard]] bool validatePage() override;

    [[nodiscard]] QUrl fileNamePath() const;

    [[nodiscard]] bool isComplete() const override;

    void loadSettings();
    void saveSettings();

private:
    CreateVideoMessageWidget *const mCreateVideoMessageWidget;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT GenerateVideoMessagePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit GenerateVideoMessagePage(RocketChatAccount *account, QWidget *parent = nullptr);
    ~GenerateVideoMessagePage() override;

    [[nodiscard]] QString fileName() const;
    [[nodiscard]] QString description() const;
    void setFileNamePath(const QString &url);
    [[nodiscard]] QUrl fileNameUrl() const;

private:
    ShowVideoWidget *const mShowVideoWidget;
    QLineEdit *const mFileName;
    QLineEdit *const mDescription;
};

LIBRUQOLAWIDGETS_TESTS_EXPORT QDebug operator<<(QDebug d, const CreateVideoMessageWizard::CreateVideoMessageInfo &t);
