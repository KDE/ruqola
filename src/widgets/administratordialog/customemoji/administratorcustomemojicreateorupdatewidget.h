/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDebug>
#include <QIcon>
#include <QUrl>
#include <QWidget>
class QLineEdit;
class KUrlRequester;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorCustomEmojiCreateOrUpdateWidget : public QWidget
{
    Q_OBJECT
public:
    enum AdministratorCustomEmojiCreateOrUpdateType {
        Create,
        Update,
    };

    struct CustomEmojiCreateInfo {
        QString alias;
        QString name;
        QUrl fileNameUrl;
        QIcon icon;
    };
    explicit AdministratorCustomEmojiCreateOrUpdateWidget(QWidget *parent = nullptr);
    ~AdministratorCustomEmojiCreateOrUpdateWidget() override;

    void setCustomEmojiInfo(const CustomEmojiCreateInfo &info);

    Q_REQUIRED_RESULT AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo info() const;

    Q_REQUIRED_RESULT AdministratorCustomEmojiCreateOrUpdateType type() const;
    void setType(AdministratorCustomEmojiCreateOrUpdateType newType);

Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    void slotUpdateOkButton();
    // Set AdministratorCustomEmojiCreateOrUpdateType as Create
    AdministratorCustomEmojiCreateOrUpdateType mType = AdministratorCustomEmojiCreateOrUpdateType::Create;
    QLineEdit *const mName;
    QLineEdit *const mAlias;
    KUrlRequester *const mSelectFile;
    QLabel *const mWarningLabel;
    QLabel *const mIconLabel;
};
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo &t);
