/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT WhatsNewComboBoxWidget : public QWidget
{
    Q_OBJECT
public:
    enum VersionType {
        AllVersion,
        Version2_0,
        Version2_1,
        Version2_2,
        LastVersion = Version2_2,
    };
    Q_ENUM(VersionType)

    explicit WhatsNewComboBoxWidget(QWidget *parent = nullptr);
    ~WhatsNewComboBoxWidget() override;

    void initializeVersion(VersionType type);

    [[nodiscard]] static QString convertVersionEnumToString(WhatsNewComboBoxWidget::VersionType type);

Q_SIGNALS:
    void versionChanged(WhatsNewComboBoxWidget::VersionType type);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void fillCombobox();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCurrentIndexChanged(int index);
    QComboBox *const mVersionComboBox;
};
