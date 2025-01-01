/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

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
    enum class AdministratorCustomEmojiCreateOrUpdateType : uint8_t {
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

    [[nodiscard]] AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo info() const;

    [[nodiscard]] AdministratorCustomEmojiCreateOrUpdateType type() const;
    void setType(AdministratorCustomEmojiCreateOrUpdateType newType);

Q_SIGNALS:
    void updateOkButton(bool enabled);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotUpdateOkButton();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAddNewEmoji();
    // Set AdministratorCustomEmojiCreateOrUpdateType as Create
    AdministratorCustomEmojiCreateOrUpdateType mType = AdministratorCustomEmojiCreateOrUpdateType::Create;
    QLineEdit *const mName;
    QLineEdit *const mAlias;
    KUrlRequester *const mSelectFile;
    QLabel *const mWarningLabel;
    QLabel *const mIconLabel;
};
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo &t);
