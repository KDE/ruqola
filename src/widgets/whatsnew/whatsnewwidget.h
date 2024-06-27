/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "whatsnewcomboboxwidget.h"
#include "whatsnewinfo.h"
#include <QWidget>
class QTextBrowser;
class LIBRUQOLAWIDGETS_TESTS_EXPORT WhatsNewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WhatsNewWidget(QWidget *parent = nullptr);
    ~WhatsNewWidget() override;

    void updateInformations();

private:
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT int currentVersion() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString generateStartEndHtml(const QString &str) const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString importantChangeStr() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString featuresChangeStr() const;
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString bugFixingChangeStr() const;
    LIBRUQOLAWIDGETS_NO_EXPORT void slotVersionChanged(int i);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString createVersionInformation(const WhatsNewInfo &info);
    LIBRUQOLAWIDGETS_NO_EXPORT void fillTranslations();
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString generateVersionHeader(int type) const;
    LIBRUQOLAWIDGETS_NO_EXPORT void fillComboBox();

    QList<WhatsNewInfo> mWhatsNewInfo;
    QTextBrowser *const mLabelInfo;
    WhatsNewComboBoxWidget *const mWhatsNewComboBoxWidget;
};
