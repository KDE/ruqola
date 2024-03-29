/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "whatsnewcomboboxwidget.h"
#include <QWidget>
class QTextBrowser;
class LIBRUQOLAWIDGETS_TESTS_EXPORT WhatsNewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WhatsNewWidget(QWidget *parent = nullptr);
    ~WhatsNewWidget() override;
    [[nodiscard]] static QString newFeaturesMD5();

    void updateInformations();

private:
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString generateVersionHeader(WhatsNewComboBoxWidget::VersionType type) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString createVersionInformationsV2_0() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString createVersionInformationsV2_1() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString createVersionInformationsV2_2() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT WhatsNewComboBoxWidget::VersionType currentVersion() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString generateStartEndHtml(const QString &str) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString importantChangeStr() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString featuresChangeStr() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString bugFixingChangeStr() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void slotVersionChanged(WhatsNewComboBoxWidget::VersionType type);
    QTextBrowser *const mLabelInfo;
    WhatsNewComboBoxWidget *const mWhatsNewComboBoxWidget;
};
