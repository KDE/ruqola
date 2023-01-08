/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "autocorrectionwidget.h"
#include "autocorrection.h"
#include "autocorrectionutils.h"
#include "import/importabstractautocorrection.h"
#include "import/importkmailautocorrection.h"
#include "import/importlibreofficeautocorrection.h"
#include "ui_autocorrectionwidget.h"

#include "widgets/lineeditcatchreturnkey.h"
#include "widgets/selectspecialchardialog.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QFileDialog>
#include <QMenu>
#include <QPointer>
#include <QTreeWidgetItem>

using namespace TextAutoCorrection;

Q_DECLARE_METATYPE(AutoCorrectionWidget::ImportFileType)
class TextAutoCorrection::AutoCorrectionWidgetPrivate
{
public:
    AutoCorrectionWidgetPrivate()
        : ui(new Ui::AutoCorrectionWidget)
    {
    }

    ~AutoCorrectionWidgetPrivate() = default;

    AutoCorrectionUtils::TypographicQuotes m_singleQuotes;
    AutoCorrectionUtils::TypographicQuotes m_doubleQuotes;
    QSet<QString> m_upperCaseExceptions;
    QSet<QString> m_twoUpperLetterExceptions;
    QHash<QString, QString> m_autocorrectEntries;
    std::unique_ptr<Ui::AutoCorrectionWidget> const ui;
    AutoCorrection *mAutoCorrection = nullptr;
    bool mWasChanged = false;
    bool mHasHtmlSupport = true;
};

AutoCorrectionWidget::AutoCorrectionWidget(QWidget *parent)
    : QWidget(parent)
    , d(new TextAutoCorrection::AutoCorrectionWidgetPrivate)
{
    d->ui->setupUi(this);

    d->ui->treeWidget->setSortingEnabled(true);
    d->ui->treeWidget->sortByColumn(0, Qt::AscendingOrder);

    d->ui->add1->setEnabled(false);
    d->ui->add2->setEnabled(false);

    TextAutoCorrection::LineEditCatchReturnKey(d->ui->find, this);
    TextAutoCorrection::LineEditCatchReturnKey(d->ui->replace, this);
    TextAutoCorrection::LineEditCatchReturnKey(d->ui->abbreviation, this);
    TextAutoCorrection::LineEditCatchReturnKey(d->ui->twoUpperLetter, this);
    connect(d->ui->autoChangeFormat, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->autoFormatUrl, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->upperCase, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->upperUpper, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->ignoreDoubleSpace, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->autoReplaceNumber, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->capitalizeDaysName, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->advancedAutocorrection, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->enabledAutocorrection, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->typographicSingleQuotes, &QCheckBox::clicked, this, &AutoCorrectionWidget::enableSingleQuotes);
    connect(d->ui->typographicDoubleQuotes, &QCheckBox::clicked, this, &AutoCorrectionWidget::enableDoubleQuotes);
    connect(d->ui->autoSuperScript, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->singleQuote1, &QPushButton::clicked, this, &AutoCorrectionWidget::selectSingleQuoteCharOpen);
    connect(d->ui->singleQuote2, &QPushButton::clicked, this, &AutoCorrectionWidget::selectSingleQuoteCharClose);
    connect(d->ui->singleDefault, &QPushButton::clicked, this, &AutoCorrectionWidget::setDefaultSingleQuotes);
    connect(d->ui->doubleQuote1, &QPushButton::clicked, this, &AutoCorrectionWidget::selectDoubleQuoteCharOpen);
    connect(d->ui->doubleQuote2, &QPushButton::clicked, this, &AutoCorrectionWidget::selectDoubleQuoteCharClose);
    connect(d->ui->doubleDefault, &QPushButton::clicked, this, &AutoCorrectionWidget::setDefaultDoubleQuotes);
    connect(d->ui->advancedAutocorrection, &QCheckBox::clicked, this, &AutoCorrectionWidget::enableAdvAutocorrection);
    connect(d->ui->addButton, &QPushButton::clicked, this, &AutoCorrectionWidget::addAutocorrectEntry);
    connect(d->ui->removeButton, &QPushButton::clicked, this, &AutoCorrectionWidget::removeAutocorrectEntry);
    connect(d->ui->treeWidget, &TextAutoCorrection::AutoCorrectionTreeWidget::itemClicked, this, &AutoCorrectionWidget::setFindReplaceText);
    connect(d->ui->treeWidget, &TextAutoCorrection::AutoCorrectionTreeWidget::deleteSelectedItems, this, &AutoCorrectionWidget::removeAutocorrectEntry);
    connect(d->ui->treeWidget, &TextAutoCorrection::AutoCorrectionTreeWidget::itemSelectionChanged, this, &AutoCorrectionWidget::updateAddRemoveButton);
    connect(d->ui->find, &QLineEdit::textChanged, this, &AutoCorrectionWidget::enableAddRemoveButton);
    connect(d->ui->replace, &QLineEdit::textChanged, this, &AutoCorrectionWidget::enableAddRemoveButton);
    connect(d->ui->abbreviation, &QLineEdit::textChanged, this, &AutoCorrectionWidget::abbreviationChanged);
    connect(d->ui->twoUpperLetter, &QLineEdit::textChanged, this, &AutoCorrectionWidget::twoUpperLetterChanged);
    connect(d->ui->add1, &QPushButton::clicked, this, &AutoCorrectionWidget::addAbbreviationEntry);
    connect(d->ui->remove1, &QPushButton::clicked, this, &AutoCorrectionWidget::removeAbbreviationEntry);
    connect(d->ui->add2, &QPushButton::clicked, this, &AutoCorrectionWidget::addTwoUpperLetterEntry);
    connect(d->ui->remove2, &QPushButton::clicked, this, &AutoCorrectionWidget::removeTwoUpperLetterEntry);
    connect(d->ui->typographicDoubleQuotes, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->typographicSingleQuotes, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->abbreviationList,
            &TextAutoCorrection::AutoCorrectionListWidget::itemSelectionChanged,
            this,
            &AutoCorrectionWidget::slotEnableDisableAbreviationList);
    connect(d->ui->abbreviationList, &TextAutoCorrection::AutoCorrectionListWidget::deleteSelectedItems, this, &AutoCorrectionWidget::removeAbbreviationEntry);
    connect(d->ui->twoUpperLetterList,
            &TextAutoCorrection::AutoCorrectionListWidget::itemSelectionChanged,
            this,
            &AutoCorrectionWidget::slotEnableDisableTwoUpperEntry);
    connect(d->ui->twoUpperLetterList,
            &TextAutoCorrection::AutoCorrectionListWidget::deleteSelectedItems,
            this,
            &AutoCorrectionWidget::removeTwoUpperLetterEntry);
    connect(d->ui->autocorrectionLanguage, &TextAutoCorrection::AutoCorrectionLanguage::activated, this, &AutoCorrectionWidget::changeLanguage);
    connect(d->ui->addNonBreakingSpaceInFrench, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->twoUpperLetter, &QLineEdit::returnPressed, this, &AutoCorrectionWidget::addTwoUpperLetterEntry);
    connect(d->ui->abbreviation, &QLineEdit::returnPressed, this, &AutoCorrectionWidget::addAbbreviationEntry);
    connect(d->ui->replaceDoubleQuotesByFrenchQuotes, &QCheckBox::clicked, this, &AutoCorrectionWidget::emitChanged);

    connect(d->ui->customWritablePath, &QLineEdit::textChanged, this, &AutoCorrectionWidget::emitChanged);
    connect(d->ui->customSystemPath, &QLineEdit::textChanged, this, &AutoCorrectionWidget::emitChanged);

    connect(d->ui->customWritablePathToolButton, &QToolButton::clicked, this, [this]() {
        const QString path = QFileDialog::getExistingDirectory(this, i18n("Select Custom Writable Path"));
        d->ui->customWritablePath->setText(path);
    });

    connect(d->ui->customSystemPathToolButton, &QToolButton::clicked, this, [this]() {
        const QString path = QFileDialog::getExistingDirectory(this, i18n("Select Custom System Path"));
        d->ui->customSystemPath->setText(path);
    });

    slotEnableDisableAbreviationList();
    slotEnableDisableTwoUpperEntry();

    auto menu = new QMenu(this);
    d->ui->importAutoCorrection->setMenu(menu);

    auto act = new QAction(i18n("LibreOffice Autocorrection"), this);
    act->setData(QVariant::fromValue(AutoCorrectionWidget::LibreOffice));
    menu->addAction(act);

    act = new QAction(i18n("KMail/Calligra Autocorrection"), this);
    act->setData(QVariant::fromValue(AutoCorrectionWidget::KMail));
    menu->addAction(act);

    connect(menu, &QMenu::triggered, this, &AutoCorrectionWidget::slotImportAutoCorrection);

    connect(d->ui->exportAutoCorrection, &QPushButton::clicked, this, &AutoCorrectionWidget::slotExportAutoCorrection);
    connect(d->ui->tabWidget, &QTabWidget::tabBarClicked, this, &AutoCorrectionWidget::slotChangeComboboxState);
    slotChangeComboboxState(d->ui->tabWidget->currentIndex());

    d->ui->systemPath->setText(TextAutoCorrection::AutoCorrectionUtils::libreOfficeSystemPath());
    d->ui->writablePath->setText(TextAutoCorrection::AutoCorrectionUtils::libreOfficeWritableLocalAutoCorrectionPath());
}

AutoCorrectionWidget::~AutoCorrectionWidget() = default;

void AutoCorrectionWidget::slotChangeComboboxState(int index)
{
    d->ui->autocorrectionLanguage->setEnabled(index < 2);
}

void AutoCorrectionWidget::setAutoCorrection(AutoCorrection *autoCorrect)
{
    d->mAutoCorrection = autoCorrect;
    setLanguage(d->ui->autocorrectionLanguage->language());
}

void AutoCorrectionWidget::setHasHtmlSupport(bool b)
{
    if (d->mHasHtmlSupport != b) {
        d->mHasHtmlSupport = b;
        updateHtmlSupport();
    }
}

void AutoCorrectionWidget::updateHtmlSupport()
{
    d->ui->autoFormatUrl->setVisible(d->mHasHtmlSupport);
    d->ui->autoSuperScript->setVisible(d->mHasHtmlSupport);
    d->ui->autoChangeFormat->setVisible(d->mHasHtmlSupport);
}

void AutoCorrectionWidget::loadConfig()
{
    if (!d->mAutoCorrection) {
        return;
    }
    AutoCorrectionSettings *settings = d->mAutoCorrection->autoCorrectionSettings();
    d->ui->autoChangeFormat->setChecked(settings->isAutoBoldUnderline());
    d->ui->autoFormatUrl->setChecked(settings->isAutoFormatUrl());
    d->ui->enabledAutocorrection->setChecked(settings->isEnabledAutoCorrection());
    d->ui->upperCase->setChecked(settings->isUppercaseFirstCharOfSentence());
    d->ui->upperUpper->setChecked(settings->isFixTwoUppercaseChars());
    d->ui->ignoreDoubleSpace->setChecked(settings->isSingleSpaces());
    d->ui->autoReplaceNumber->setChecked(settings->isAutoFractions());
    d->ui->capitalizeDaysName->setChecked(settings->isCapitalizeWeekDays());
    d->ui->advancedAutocorrection->setChecked(settings->isAdvancedAutocorrect());
    d->ui->autoSuperScript->setChecked(settings->isSuperScript());
    d->ui->typographicDoubleQuotes->setChecked(settings->isReplaceDoubleQuotes());
    d->ui->typographicSingleQuotes->setChecked(settings->isReplaceSingleQuotes());
    d->ui->addNonBreakingSpaceInFrench->setChecked(settings->isAddNonBreakingSpace());
    d->ui->replaceDoubleQuotesByFrenchQuotes->setChecked(settings->isReplaceDoubleQuotesByFrenchQuotes());

    d->ui->customWritablePath->setText(settings->customWritablePath());
    d->ui->customSystemPath->setText(settings->customSystemPath());

    loadAutoCorrectionAndException();
    d->mWasChanged = false;
}

void AutoCorrectionWidget::loadAutoCorrectionAndException()
{
    /* tab 2 - Custom Quotes */
    AutoCorrectionSettings *settings = d->mAutoCorrection->autoCorrectionSettings();
    d->m_singleQuotes = settings->typographicSingleQuotes();
    d->ui->simpleQuoteBeginReplace->setText(d->m_singleQuotes.begin);
    d->ui->simpleQuoteEndReplace->setText(d->m_singleQuotes.end);
    d->m_doubleQuotes = settings->typographicDoubleQuotes();
    d->ui->doubleQuoteBeginReplace->setText(d->m_doubleQuotes.begin);
    d->ui->doubleQuoteEndReplace->setText(d->m_doubleQuotes.end);
    enableSingleQuotes(d->ui->typographicSingleQuotes->isChecked());
    enableDoubleQuotes(d->ui->typographicDoubleQuotes->isChecked());

    /* tab 3 - Advanced Autocorrection */
    d->m_autocorrectEntries = settings->autocorrectEntries();
    addAutoCorrectEntries();

    enableAdvAutocorrection(d->ui->advancedAutocorrection->isChecked());
    /* tab 4 - Exceptions */
    d->m_upperCaseExceptions = settings->upperCaseExceptions();
    d->m_twoUpperLetterExceptions = settings->twoUpperLetterExceptions();

    d->ui->twoUpperLetterList->clear();
    d->ui->twoUpperLetterList->addItems(d->m_twoUpperLetterExceptions.values());

    d->ui->abbreviationList->clear();
    d->ui->abbreviationList->addItems(d->m_upperCaseExceptions.values());
}

void AutoCorrectionWidget::addAutoCorrectEntries()
{
    d->ui->treeWidget->clear();
    QHash<QString, QString>::const_iterator i = d->m_autocorrectEntries.constBegin();
    QTreeWidgetItem *item = nullptr;
    while (i != d->m_autocorrectEntries.constEnd()) {
        item = new QTreeWidgetItem(d->ui->treeWidget, item);
        item->setText(0, i.key());
        item->setText(1, i.value());
        ++i;
    }
    d->ui->treeWidget->setSortingEnabled(true);
    d->ui->treeWidget->sortByColumn(0, Qt::AscendingOrder);
}

void AutoCorrectionWidget::writeConfig()
{
    if (!d->mAutoCorrection) {
        return;
    }
    AutoCorrectionSettings *settings = d->mAutoCorrection->autoCorrectionSettings();
    settings->setAutoBoldUnderline(d->ui->autoChangeFormat->isChecked());
    settings->setAutoFormatUrl(d->ui->autoFormatUrl->isChecked());
    settings->setEnabledAutoCorrection(d->ui->enabledAutocorrection->isChecked());
    settings->setUppercaseFirstCharOfSentence(d->ui->upperCase->isChecked());
    settings->setFixTwoUppercaseChars(d->ui->upperUpper->isChecked());
    settings->setSingleSpaces(d->ui->ignoreDoubleSpace->isChecked());
    settings->setCapitalizeWeekDays(d->ui->capitalizeDaysName->isChecked());
    settings->setAdvancedAutocorrect(d->ui->advancedAutocorrection->isChecked());
    settings->setSuperScript(d->ui->autoSuperScript->isChecked());

    settings->setAutoFractions(d->ui->autoReplaceNumber->isChecked());

    settings->setAutocorrectEntries(d->m_autocorrectEntries);
    settings->setUpperCaseExceptions(d->m_upperCaseExceptions);
    settings->setTwoUpperLetterExceptions(d->m_twoUpperLetterExceptions);

    settings->setReplaceDoubleQuotes(d->ui->typographicDoubleQuotes->isChecked());
    settings->setReplaceSingleQuotes(d->ui->typographicSingleQuotes->isChecked());
    settings->setTypographicSingleQuotes(d->m_singleQuotes);
    settings->setTypographicDoubleQuotes(d->m_doubleQuotes);
    settings->setAddNonBreakingSpace(d->ui->addNonBreakingSpaceInFrench->isChecked());
    settings->setReplaceDoubleQuotesByFrenchQuotes(d->ui->replaceDoubleQuotesByFrenchQuotes->isChecked());

    settings->setCustomWritablePath(d->ui->customWritablePath->text());
    settings->setCustomSystemPath(d->ui->customSystemPath->text());

    d->mAutoCorrection->setAutoCorrectionSettings(settings);
    d->mAutoCorrection->writeConfig();
    d->mWasChanged = false;
}

void AutoCorrectionWidget::resetToDefault()
{
    d->ui->autoChangeFormat->setChecked(false);
    d->ui->autoFormatUrl->setChecked(false);
    d->ui->upperCase->setChecked(false);
    d->ui->upperUpper->setChecked(false);
    d->ui->ignoreDoubleSpace->setChecked(false);
    d->ui->capitalizeDaysName->setChecked(false);
    d->ui->advancedAutocorrection->setChecked(false);
    d->ui->typographicDoubleQuotes->setChecked(false);
    d->ui->typographicSingleQuotes->setChecked(false);
    d->ui->autoSuperScript->setChecked(false);
    d->ui->autoReplaceNumber->setChecked(false);
    d->ui->typographicDoubleQuotes->setChecked(false);
    d->ui->typographicSingleQuotes->setChecked(false);
    d->ui->addNonBreakingSpaceInFrench->setChecked(false);

    loadGlobalAutoCorrectionAndException();
}

void AutoCorrectionWidget::enableSingleQuotes(bool state)
{
    d->ui->singleQuote1->setEnabled(state);
    d->ui->singleQuote2->setEnabled(state);
    d->ui->singleDefault->setEnabled(state);
}

void AutoCorrectionWidget::enableDoubleQuotes(bool state)
{
    d->ui->doubleQuote1->setEnabled(state);
    d->ui->doubleQuote2->setEnabled(state);
    d->ui->doubleDefault->setEnabled(state);
}

void AutoCorrectionWidget::selectSingleQuoteCharOpen()
{
    QPointer<TextAutoCorrection::SelectSpecialCharDialog> dlg = new TextAutoCorrection::SelectSpecialCharDialog(this);
    dlg->setCurrentChar(d->m_singleQuotes.begin);
    dlg->showSelectButton(false);
    dlg->autoInsertChar();
    if (dlg->exec()) {
        d->m_singleQuotes.begin = dlg->currentChar();
        d->ui->simpleQuoteBeginReplace->setText(d->m_singleQuotes.begin);
        emitChanged();
    }
    delete dlg;
}

void AutoCorrectionWidget::selectSingleQuoteCharClose()
{
    QPointer<TextAutoCorrection::SelectSpecialCharDialog> dlg = new TextAutoCorrection::SelectSpecialCharDialog(this);
    dlg->showSelectButton(false);
    dlg->setCurrentChar(d->m_singleQuotes.end);
    dlg->autoInsertChar();
    if (dlg->exec()) {
        d->m_singleQuotes.end = dlg->currentChar();
        d->ui->simpleQuoteEndReplace->setText(d->m_singleQuotes.end);
        emitChanged();
    }
    delete dlg;
}

void AutoCorrectionWidget::setDefaultSingleQuotes()
{
    d->m_singleQuotes = AutoCorrectionUtils::typographicDefaultSingleQuotes();
    d->ui->simpleQuoteBeginReplace->setText(d->m_singleQuotes.begin);
    d->ui->simpleQuoteEndReplace->setText(d->m_singleQuotes.end);
    emitChanged();
}

void AutoCorrectionWidget::selectDoubleQuoteCharOpen()
{
    QPointer<TextAutoCorrection::SelectSpecialCharDialog> dlg = new TextAutoCorrection::SelectSpecialCharDialog(this);
    dlg->showSelectButton(false);
    dlg->setCurrentChar(d->m_doubleQuotes.begin);
    dlg->autoInsertChar();
    if (dlg->exec()) {
        d->m_doubleQuotes.begin = dlg->currentChar();
        d->ui->doubleQuoteBeginReplace->setText(d->m_doubleQuotes.begin);
        emitChanged();
    }
    delete dlg;
}

void AutoCorrectionWidget::selectDoubleQuoteCharClose()
{
    QPointer<TextAutoCorrection::SelectSpecialCharDialog> dlg = new TextAutoCorrection::SelectSpecialCharDialog(this);
    dlg->showSelectButton(false);
    dlg->setCurrentChar(d->m_doubleQuotes.end);
    dlg->autoInsertChar();
    if (dlg->exec()) {
        d->m_doubleQuotes.end = dlg->currentChar();
        d->ui->doubleQuoteEndReplace->setText(d->m_doubleQuotes.end);
        emitChanged();
    }
    delete dlg;
}

void AutoCorrectionWidget::setDefaultDoubleQuotes()
{
    d->m_doubleQuotes = AutoCorrectionUtils::typographicDefaultDoubleQuotes();
    d->ui->doubleQuoteBeginReplace->setText(d->m_doubleQuotes.begin);
    d->ui->doubleQuoteEndReplace->setText(d->m_doubleQuotes.end);
    emitChanged();
}

void AutoCorrectionWidget::enableAdvAutocorrection(bool state)
{
    d->ui->findLabel->setEnabled(state);
    d->ui->find->setEnabled(state);
    d->ui->replaceLabel->setEnabled(state);
    d->ui->replace->setEnabled(state);

    const QString find = d->ui->find->text();
    const QString replace = d->ui->replace->text();

    d->ui->addButton->setEnabled(state && !find.isEmpty() && !replace.isEmpty());
    d->ui->removeButton->setEnabled(state && d->ui->treeWidget->currentItem());
    d->ui->treeWidget->setEnabled(state);
}

void AutoCorrectionWidget::addAutocorrectEntry()
{
    QTreeWidgetItem *item = d->ui->treeWidget->currentItem();
    const QString find = d->ui->find->text();
    const QString replace = d->ui->replace->text();
    if (find == replace) {
        KMessageBox::error(this, i18n("\"Replace\" string is the same as \"Find\" string."), i18n("Add Autocorrection Entry"));
        return;
    }

    bool modify = false;

    // Modify actually, not add, so we want to remove item from hash
    if (item && (find == item->text(0))) {
        d->m_autocorrectEntries.remove(find);
        modify = true;
    }

    d->m_autocorrectEntries.insert(find, replace);
    d->ui->treeWidget->setSortingEnabled(false);
    if (modify) {
        item->setText(0, find);
        item->setText(1, replace);
    } else {
        item = new QTreeWidgetItem(d->ui->treeWidget, item);
        item->setText(0, find);
        item->setText(1, replace);
    }

    d->ui->treeWidget->setSortingEnabled(true);
    d->ui->treeWidget->setCurrentItem(item);
    emitChanged();
}

void AutoCorrectionWidget::removeAutocorrectEntry()
{
    const QList<QTreeWidgetItem *> listItems = d->ui->treeWidget->selectedItems();
    if (listItems.isEmpty()) {
        return;
    }
    for (QTreeWidgetItem *item : listItems) {
        QTreeWidgetItem *below = d->ui->treeWidget->itemBelow(item);

        QString findStr;
        if (below) {
            // qCDebug(TEXTAUTOCORRECTION_AUTOCORRECT_LOG) << "below";
            findStr = item->text(0);
            delete item;
            item = nullptr;
        } else if (d->ui->treeWidget->topLevelItemCount() > 0) {
            findStr = item->text(0);
            delete item;
            item = nullptr;
        }
        if (!findStr.isEmpty()) {
            d->m_autocorrectEntries.remove(findStr);
        }
    }
    d->ui->treeWidget->setSortingEnabled(false);

    emitChanged();
}

void AutoCorrectionWidget::updateAddRemoveButton()
{
    QList<QTreeWidgetItem *> listItems = d->ui->treeWidget->selectedItems();
    d->ui->removeButton->setEnabled(!listItems.isEmpty());
}

void AutoCorrectionWidget::enableAddRemoveButton()
{
    const QString find = d->ui->find->text();
    const QString replace = d->ui->replace->text();

    QTreeWidgetItem *item = nullptr;
    if (d->m_autocorrectEntries.contains(find)) {
        item = d->ui->treeWidget->findItems(find, Qt::MatchCaseSensitive).at(0);
    }

    bool enable = false;
    if (find.isEmpty() || replace.isEmpty()) { // disable if no text in find/replace
        enable = !(find.isEmpty() || replace.isEmpty());
    } else if (item && find == item->text(0)) {
        // We disable add / remove button if no text for the replacement
        enable = !item->text(1).isEmpty();
        d->ui->addButton->setText(i18n("&Modify"));
    } else if (!item || !item->text(1).isEmpty()) {
        enable = true;
        d->ui->addButton->setText(i18n("&Add"));
    }

    if (item && replace == item->text(1)) {
        d->ui->addButton->setEnabled(false);
    } else {
        d->ui->addButton->setEnabled(enable);
    }
    d->ui->removeButton->setEnabled(enable);
}

void AutoCorrectionWidget::setFindReplaceText(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    d->ui->find->setText(item->text(0));
    d->ui->replace->setText(item->text(1));
}

void AutoCorrectionWidget::abbreviationChanged(const QString &text)
{
    d->ui->add1->setEnabled(!text.isEmpty());
}

void AutoCorrectionWidget::twoUpperLetterChanged(const QString &text)
{
    d->ui->add2->setEnabled(!text.isEmpty());
}

void AutoCorrectionWidget::addAbbreviationEntry()
{
    const QString text = d->ui->abbreviation->text();
    if (text.isEmpty()) {
        return;
    }
    if (!d->m_upperCaseExceptions.contains(text)) {
        d->m_upperCaseExceptions.insert(text);
        d->ui->abbreviationList->addItem(text);
    }
    d->ui->abbreviation->clear();
    slotEnableDisableAbreviationList();
    emitChanged();
}

void AutoCorrectionWidget::removeAbbreviationEntry()
{
    const QList<QListWidgetItem *> listItem = d->ui->abbreviationList->selectedItems();
    if (listItem.isEmpty()) {
        return;
    }
    for (QListWidgetItem *item : listItem) {
        d->m_upperCaseExceptions.remove(item->text());
        delete item;
    }
    slotEnableDisableAbreviationList();
    emitChanged();
}

void AutoCorrectionWidget::addTwoUpperLetterEntry()
{
    const QString text = d->ui->twoUpperLetter->text();
    if (text.isEmpty()) {
        return;
    }
    if (!d->m_twoUpperLetterExceptions.contains(text)) {
        d->m_twoUpperLetterExceptions.insert(text);
        d->ui->twoUpperLetterList->addItem(text);
        emitChanged();
    }
    slotEnableDisableTwoUpperEntry();
    d->ui->twoUpperLetter->clear();
}

void AutoCorrectionWidget::removeTwoUpperLetterEntry()
{
    const QList<QListWidgetItem *> listItem = d->ui->twoUpperLetterList->selectedItems();
    if (listItem.isEmpty()) {
        return;
    }
    for (QListWidgetItem *item : listItem) {
        d->m_twoUpperLetterExceptions.remove(item->text());
        delete item;
    }
    slotEnableDisableTwoUpperEntry();
    emitChanged();
}

void AutoCorrectionWidget::slotEnableDisableAbreviationList()
{
    const bool enable = (!d->ui->abbreviationList->selectedItems().isEmpty());
    d->ui->add1->setEnabled(!d->ui->abbreviation->text().isEmpty());
    d->ui->remove1->setEnabled(enable);
}

void AutoCorrectionWidget::slotEnableDisableTwoUpperEntry()
{
    const bool enable = (!d->ui->twoUpperLetterList->selectedItems().isEmpty());
    d->ui->add2->setEnabled(!d->ui->twoUpperLetter->text().isEmpty());
    d->ui->remove2->setEnabled(enable);
}

void AutoCorrectionWidget::slotImportAutoCorrection(QAction *act)
{
    if (act) {
        const auto type = act->data().value<AutoCorrectionWidget::ImportFileType>();
        QString title;
        QString filter;
        switch (type) {
        case AutoCorrectionWidget::LibreOffice:
            title = i18n("Import LibreOffice Autocorrection");
            filter = i18n("LibreOffice Autocorrection File (*.dat)");
            break;
        case AutoCorrectionWidget::KMail:
            title = i18n("Import KMail Autocorrection");
            filter = i18n("KMail Autocorrection File (*.xml)");
            break;
        }
        const QString fileName = QFileDialog::getOpenFileName(this, title, QString(), filter);
        if (!fileName.isEmpty()) {
            TextAutoCorrection::ImportAbstractAutocorrection *importAutoCorrection = nullptr;
            switch (type) {
            case AutoCorrectionWidget::LibreOffice:
                importAutoCorrection = new TextAutoCorrection::ImportLibreOfficeAutocorrection;
                break;
            case AutoCorrectionWidget::KMail:
                importAutoCorrection = new TextAutoCorrection::ImportKMailAutocorrection;
                break;
            default:
                return;
            }
            QString messageError;
            if (importAutoCorrection->import(fileName, messageError, ImportAbstractAutocorrection::All)) {
                d->m_autocorrectEntries = importAutoCorrection->autocorrectEntries();
                addAutoCorrectEntries();

                enableAdvAutocorrection(d->ui->advancedAutocorrection->isChecked());

                d->m_upperCaseExceptions = importAutoCorrection->upperCaseExceptions();
                d->m_twoUpperLetterExceptions = importAutoCorrection->twoUpperLetterExceptions();

                d->ui->twoUpperLetterList->clear();
                d->ui->twoUpperLetterList->addItems(d->m_twoUpperLetterExceptions.values());

                d->ui->abbreviationList->clear();
                d->ui->abbreviationList->addItems(d->m_upperCaseExceptions.values());
            } else {
                KMessageBox::error(this, messageError, i18n("Import Autocorrection File"));
            }
            delete importAutoCorrection;
        }
    }
}

void AutoCorrectionWidget::setLanguage(const QString &lang)
{
    TextAutoCorrection::AutoCorrectionSettings *settings = d->mAutoCorrection->autoCorrectionSettings();
    settings->setLanguage(lang);
    d->mAutoCorrection->setAutoCorrectionSettings(settings);
    loadAutoCorrectionAndException();
    d->mWasChanged = false;
}

void AutoCorrectionWidget::changeLanguage(int index)
{
    if (index == -1) {
        return;
    }
    if (d->mWasChanged) {
        const int rc = KMessageBox::warningTwoActions(this,
                                                      i18n("Language was changed, do you want to save config for previous language?"),
                                                      i18n("Save config"),
                                                      KStandardGuiItem::save(),
                                                      KStandardGuiItem::discard());
        if (rc == KMessageBox::ButtonCode::PrimaryAction) {
            writeConfig();
        }
    }
    const QString lang = d->ui->autocorrectionLanguage->itemData(index).toString();
    TextAutoCorrection::AutoCorrectionSettings *settings = d->mAutoCorrection->autoCorrectionSettings();
    settings->setLanguage(lang);
    d->mAutoCorrection->setAutoCorrectionSettings(settings);
    loadAutoCorrectionAndException();
    d->mWasChanged = false;
}

void AutoCorrectionWidget::emitChanged()
{
    d->mWasChanged = true;
    Q_EMIT changed();
}

void AutoCorrectionWidget::loadGlobalAutoCorrectionAndException()
{
    const QString lang = d->ui->autocorrectionLanguage->itemData(d->ui->autocorrectionLanguage->currentIndex()).toString();
    TextAutoCorrection::AutoCorrectionSettings *settings = d->mAutoCorrection->autoCorrectionSettings();
    settings->setLanguage(lang, true);
    d->mAutoCorrection->setAutoCorrectionSettings(settings);
    loadAutoCorrectionAndException();
    d->mWasChanged = true;
    Q_EMIT changed();
}

void AutoCorrectionWidget::slotExportAutoCorrection()
{
    if (d->mWasChanged) {
        KMessageBox::error(this, i18n("Please save changes before exporting settings."), i18n("Export Autocorrection File"));
        return;
    }
    const QString saveUrl = QFileDialog::getSaveFileName(this, i18n("Export Autocorrection File"), QDir::homePath());
    if (saveUrl.isEmpty()) {
        return;
    }
    d->mAutoCorrection->writeAutoCorrectionXmlFile(saveUrl);
}
