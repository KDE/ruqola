/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "whatsnewutils.h"
#include <QWidget>
class QComboBox;
class LIBRUQOLAWIDGETS_TESTS_EXPORT WhatsNewComboBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WhatsNewComboBoxWidget(QWidget *parent = nullptr);
    ~WhatsNewComboBoxWidget() override;

    void initializeVersion(WhatsNewUtils::VersionType type);

Q_SIGNALS:
    void versionChanged(WhatsNewUtils::VersionType type);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void fillCombobox();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotCurrentIndexChanged(int index);
    QComboBox *const mVersionComboBox;
};
