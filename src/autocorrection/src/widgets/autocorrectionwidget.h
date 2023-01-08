/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "autocorrection.h"
#include "textautocorrection_export.h"
#include <QWidget>

class QTreeWidgetItem;

namespace Ui
{
class AutoCorrectionWidget;
}

namespace TextAutoCorrection
{
class AutoCorrectionWidgetPrivate;
/**
 * @brief The AutoCorrectionWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTAUTOCORRECTION_EXPORT AutoCorrectionWidget : public QWidget
{
    Q_OBJECT

public:
    enum ImportFileType {
        LibreOffice,
        KMail,
    };

    explicit AutoCorrectionWidget(QWidget *parent = nullptr);
    ~AutoCorrectionWidget() override;
    void setAutoCorrection(AutoCorrection *autoCorrect);
    void setHasHtmlSupport(bool b);
    void loadConfig();
    void writeConfig();
    void resetToDefault();

Q_SIGNALS:
    void changed();

private:
    /* tab 2 */
    void enableSingleQuotes(bool state);
    void enableDoubleQuotes(bool state);
    void selectSingleQuoteCharOpen();
    void selectSingleQuoteCharClose();
    void setDefaultSingleQuotes();
    void selectDoubleQuoteCharOpen();
    void selectDoubleQuoteCharClose();
    void setDefaultDoubleQuotes();

    /* tab 3 */
    void enableAdvAutocorrection(bool state);
    void addAutocorrectEntry();
    void removeAutocorrectEntry();
    void setFindReplaceText(QTreeWidgetItem *, int);
    void enableAddRemoveButton();

    /* tab 4 */
    void abbreviationChanged(const QString &text);
    void twoUpperLetterChanged(const QString &text);
    void addAbbreviationEntry();
    void removeAbbreviationEntry();
    void addTwoUpperLetterEntry();
    void removeTwoUpperLetterEntry();

    void slotEnableDisableAbreviationList();
    void slotEnableDisableTwoUpperEntry();

    void slotImportAutoCorrection(QAction *act);

    void changeLanguage(int);
    void updateAddRemoveButton();

    void slotExportAutoCorrection();

private:
    void slotChangeComboboxState(int index);
    void emitChanged();
    void addAutoCorrectEntries();
    void loadAutoCorrectionAndException();
    void loadGlobalAutoCorrectionAndException();
    void setLanguage(const QString &lang);
    void updateHtmlSupport();

    std::unique_ptr<AutoCorrectionWidgetPrivate> const d;
};
}
