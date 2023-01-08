/*

  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorwidget.h"
#include "texttranslator_debug.h"
#include "translator/misc/translatorutil.h"
#include "translator/networkmanager.h"
#include "translator/translatorengineclient.h"
#include "translator/translatorengineloader.h"
#include "translator/translatorengineplugin.h"
#include "translatorconfiguredialog.h"
#include "translatordebugdialog.h"
#include <KBusyIndicatorWidget>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSeparator>
#include <QPushButton>

#include <KSharedConfig>
#include <QComboBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QKeyEvent>
#include <QLabel>
#include <QMimeData>
#include <QPainter>
#include <QShortcut>
#include <QSplitter>
#include <QToolButton>
#include <QVBoxLayout>

using namespace TextTranslator;
namespace
{
static const char myTranslatorWidgetConfigGroupName[] = "TranslatorWidget";
}
class Q_DECL_HIDDEN TranslatorWidget::TranslatorWidgetPrivate
{
public:
    TranslatorWidgetPrivate() = default;

    ~TranslatorWidgetPrivate()
    {
        delete translatorPlugin;
    }

    void initLanguage();
    void fillToCombobox(const QString &lang);

    QMap<TranslatorUtil::Language, QString> listLanguage;
    QByteArray data;
    TranslatorTextEdit *inputText = nullptr;
    TranslatorResultTextEdit *translatorResultTextEdit = nullptr;
    QComboBox *fromCombobox = nullptr;
    QComboBox *toCombobox = nullptr;
    QPushButton *translate = nullptr;
    QPushButton *clear = nullptr;
    QLabel *engineNameLabel = nullptr;
    TextTranslator::TranslatorEngineClient *translatorClient = nullptr;
    TextTranslator::TranslatorEnginePlugin *translatorPlugin = nullptr;
    KBusyIndicatorWidget *progressIndicator = nullptr;
    QPushButton *invert = nullptr;
    QSplitter *splitter = nullptr;
    QString engineName;
    bool languageSettingsChanged = false;
    bool standalone = true;
};

void TranslatorWidget::TranslatorWidgetPrivate::fillToCombobox(const QString &lang)
{
    toCombobox->clear();

    TranslatorUtil translatorUtil;
    QMapIterator<TranslatorUtil::Language, QString> i(listLanguage);
    while (i.hasNext()) {
        i.next();
        const QString languageCode = TranslatorUtil::languageCode(i.key());
        if ((i.key() != TranslatorUtil::automatic) && languageCode != lang) {
            translatorUtil.addItemToFromComboBox(toCombobox, languageCode, i.value());
        }
    }
}

void TranslatorWidget::TranslatorWidgetPrivate::initLanguage()
{
    if (!translatorClient) {
        return;
    }
    toCombobox->clear();
    fromCombobox->clear();
    listLanguage = translatorClient->supportedLanguages();

    QMapIterator<TranslatorUtil::Language, QString> i(listLanguage);
    TranslatorUtil translatorUtil;
    while (i.hasNext()) {
        i.next();
        const QString languageCode = TranslatorUtil::languageCode(i.key());
        translatorUtil.addItemToFromComboBox(fromCombobox, languageCode, i.value());
    }
}

TranslatorResultTextEdit::TranslatorResultTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setReadOnly(true);
}

void TranslatorResultTextEdit::setResultFailed(bool failed)
{
    if (mResultFailed != failed) {
        mResultFailed = failed;
        update();
    }
}

void TranslatorResultTextEdit::paintEvent(QPaintEvent *event)
{
    if (mResultFailed) {
        QPainter p(viewport());

        QFont font = p.font();
        font.setItalic(true);
        p.setFont(font);

        p.setPen(Qt::red);

        p.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, i18n("Problem when connecting to the translator web site."));
    } else {
        QPlainTextEdit::paintEvent(event);
    }
}

TranslatorTextEdit::TranslatorTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
}

void TranslatorTextEdit::dropEvent(QDropEvent *event)
{
    if (event->source() != this) {
        if (event->mimeData()->hasText()) {
            QTextCursor cursor = textCursor();
            cursor.beginEditBlock();
            cursor.insertText(event->mimeData()->text());
            cursor.endEditBlock();
            event->setDropAction(Qt::CopyAction);
            event->accept();
            Q_EMIT translateText();
            return;
        }
    }
    QPlainTextEdit::dropEvent(event);
}

TranslatorWidget::TranslatorWidget(QWidget *parent)
    : QWidget(parent)
    , d(new TranslatorWidgetPrivate)
{
    init();
}

TranslatorWidget::TranslatorWidget(const QString &text, QWidget *parent)
    : QWidget(parent)
    , d(new TranslatorWidgetPrivate)
{
    init();
    d->inputText->setPlainText(text);
}

TranslatorWidget::~TranslatorWidget()
{
    disconnect(d->inputText, &TranslatorTextEdit::textChanged, this, &TranslatorWidget::slotTextChanged);
    disconnect(d->inputText, &TranslatorTextEdit::translateText, this, &TranslatorWidget::slotTranslate);
    writeConfig();
}

void TranslatorWidget::writeConfig()
{
    if (d->languageSettingsChanged) {
        KConfigGroup myGroup(KSharedConfig::openConfig(), QStringLiteral("General"));
        myGroup.writeEntry(QStringLiteral("FromLanguage"), d->fromCombobox->itemData(d->fromCombobox->currentIndex()).toString());
        myGroup.writeEntry("ToLanguage", d->toCombobox->itemData(d->toCombobox->currentIndex()).toString());
        myGroup.sync();
    }
    KConfigGroup myGroupUi(KSharedConfig::openStateConfig(), myTranslatorWidgetConfigGroupName);
    myGroupUi.writeEntry("mainSplitter", d->splitter->sizes());
    myGroupUi.sync();
}

void TranslatorWidget::readConfig()
{
    KConfigGroup myGroupUi(KSharedConfig::openStateConfig(), myTranslatorWidgetConfigGroupName);
    const QList<int> size = {100, 400};
    d->splitter->setSizes(myGroupUi.readEntry("mainSplitter", size));

    KConfigGroup myGroup(KSharedConfig::openConfig(), QStringLiteral("General"));
    const QString from = myGroup.readEntry(QStringLiteral("FromLanguage"));
    if (from.isEmpty()) {
        return;
    }
    const QString to = myGroup.readEntry(QStringLiteral("ToLanguage"));
    const int indexFrom = d->fromCombobox->findData(from);
    if (indexFrom != -1) {
        d->fromCombobox->setCurrentIndex(indexFrom);
    }
    const int indexTo = d->toCombobox->findData(to);
    if (indexTo != -1) {
        d->toCombobox->setCurrentIndex(indexTo);
    }
    d->invert->setEnabled(from != QLatin1String("auto"));
}

void TranslatorWidget::loadEngineSettings()
{
    d->engineName = TranslatorUtil::loadEngine();
    // TODO fallback if name is empty ?
    switchEngine();
}

void TranslatorWidget::init()
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins({});
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    auto closeBtn = new QToolButton(this);
    closeBtn->setObjectName(QStringLiteral("close-button"));
    closeBtn->setIcon(QIcon::fromTheme(QStringLiteral("dialog-close")));
    closeBtn->setIconSize(QSize(16, 16));
    closeBtn->setToolTip(i18n("Close"));

#ifndef QT_NO_ACCESSIBILITY
    closeBtn->setAccessibleName(i18n("Close"));
#endif
    closeBtn->setAutoRaise(true);
    hboxLayout->addWidget(closeBtn);
    connect(closeBtn, &QToolButton::clicked, this, &TranslatorWidget::slotCloseWidget);

    auto label = new QLabel(i18nc("Translate from language", "From:"), this);
    hboxLayout->addWidget(label);
    d->fromCombobox = new QComboBox(this);
    d->fromCombobox->setMinimumWidth(50);
    d->fromCombobox->setObjectName(QStringLiteral("from"));
    hboxLayout->addWidget(d->fromCombobox);

    label = new QLabel(i18nc("Translate to language", "To:"), this);
    hboxLayout->addWidget(label);
    d->toCombobox = new QComboBox(this);
    d->toCombobox->setMinimumWidth(50);
    d->toCombobox->setObjectName(QStringLiteral("to"));

    hboxLayout->addWidget(d->toCombobox);

    auto separator = new KSeparator(this);
    separator->setOrientation(Qt::Vertical);
    hboxLayout->addWidget(separator);

    d->invert = new QPushButton(i18nc("Invert language choices so that from becomes to and to becomes from", "Invert"), this);
    d->invert->setObjectName(QStringLiteral("invert-button"));
    connect(d->invert, &QPushButton::clicked, this, &TranslatorWidget::slotInvertLanguage);
    hboxLayout->addWidget(d->invert);

    d->clear = new QPushButton(i18n("Clear"), this);
    d->clear->setObjectName(QStringLiteral("clear-button"));
#ifndef QT_NO_ACCESSIBILITY
    d->clear->setAccessibleName(i18n("Clear"));
#endif
    connect(d->clear, &QPushButton::clicked, this, &TranslatorWidget::slotClear);
    hboxLayout->addWidget(d->clear);

    d->translate = new QPushButton(i18n("Translate"), this);
    d->translate->setObjectName(QStringLiteral("translate-button"));
#ifndef QT_NO_ACCESSIBILITY
    d->translate->setAccessibleName(i18n("Translate"));
#endif

    hboxLayout->addWidget(d->translate);
    connect(d->translate, &QPushButton::clicked, this, &TranslatorWidget::slotTranslate);

    if (!qEnvironmentVariableIsEmpty("TRANSLATING_DEBUGGING")) {
        auto debugButton = new QPushButton(i18n("Debug"));
        hboxLayout->addWidget(debugButton);
        connect(debugButton, &QPushButton::clicked, this, &TranslatorWidget::slotDebug);
    }

    d->progressIndicator = new KBusyIndicatorWidget(this);
    hboxLayout->addWidget(d->progressIndicator);
    d->progressIndicator->setFixedHeight(d->toCombobox->height());

    hboxLayout->addStretch();

    d->engineNameLabel = new QLabel(this);
    hboxLayout->addWidget(d->engineNameLabel);

    auto configureButton = new QToolButton(this);
    configureButton->setObjectName(QStringLiteral("configure_button"));
    configureButton->setIcon(QIcon::fromTheme(QStringLiteral("configure")));
    configureButton->setIconSize(QSize(16, 16));
    configureButton->setToolTip(i18n("Configure"));
    connect(configureButton, &QToolButton::clicked, this, [this]() {
        TranslatorConfigureDialog dlg(this);
        if (dlg.exec()) {
            loadEngineSettings();
        }
    });
    hboxLayout->addWidget(configureButton);

    layout->addLayout(hboxLayout);

    d->splitter = new QSplitter;
    d->splitter->setChildrenCollapsible(false);
    d->inputText = new TranslatorTextEdit(this);
    d->inputText->setObjectName(QStringLiteral("inputtext"));
    d->inputText->setPlaceholderText(i18n("Drag text that you want to translate. (Be careful text will be send to Google Server)."));
    connect(d->inputText, &TranslatorTextEdit::textChanged, this, &TranslatorWidget::slotTextChanged);
    connect(d->inputText, &TranslatorTextEdit::translateText, this, &TranslatorWidget::slotTranslate);

    d->splitter->addWidget(d->inputText);
    d->translatorResultTextEdit = new TranslatorResultTextEdit(this);
    d->translatorResultTextEdit->setObjectName(QStringLiteral("translatedtext"));
    d->translatorResultTextEdit->setReadOnly(true);
    d->splitter->addWidget(d->translatorResultTextEdit);

    layout->addWidget(d->splitter);

    d->fromCombobox->setCurrentIndex(0); // Fill "to" combobox
    loadEngineSettings();
    switchEngine();
    slotFromLanguageChanged(0, true);
    slotTextChanged();
    readConfig();
    connect(d->fromCombobox, &QComboBox::currentIndexChanged, this, [this](int val) {
        slotFromLanguageChanged(val, false);
        slotConfigChanged();
    });
    connect(d->toCombobox, &QComboBox::currentIndexChanged, this, [this]() {
        slotConfigChanged();
        slotTranslate();
    });

    hide();
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum));
    d->languageSettingsChanged = false;
}

void TranslatorWidget::switchEngine()
{
    if (d->translatorPlugin) {
        disconnect(d->translatorPlugin);
        delete d->translatorPlugin;
        d->translatorPlugin = nullptr;
    }
    d->translatorClient = TextTranslator::TranslatorEngineLoader::self()->createTranslatorClient(d->engineName);
    if (!d->translatorClient) {
        const QString fallBackEngineName = TextTranslator::TranslatorEngineLoader::self()->fallbackFirstEngine();
        if (!fallBackEngineName.isEmpty()) {
            d->translatorClient = TextTranslator::TranslatorEngineLoader::self()->createTranslatorClient(fallBackEngineName);
        }
    }
    if (d->translatorClient) {
        d->translatorPlugin = d->translatorClient->createTranslator();
        connect(d->translatorPlugin, &TextTranslator::TranslatorEnginePlugin::translateDone, this, &TranslatorWidget::slotTranslateDone);
        connect(d->translatorPlugin, &TextTranslator::TranslatorEnginePlugin::translateFailed, this, &TranslatorWidget::slotTranslateFailed);
        d->initLanguage();
        d->engineNameLabel->setText(QStringLiteral("[%1]").arg(d->translatorClient->translatedName()));
    }
}

void TranslatorWidget::slotConfigChanged()
{
    d->languageSettingsChanged = true;
}

void TranslatorWidget::slotTextChanged()
{
    d->translate->setEnabled(!d->inputText->document()->isEmpty());
    d->clear->setEnabled(!d->inputText->document()->isEmpty());
}

void TranslatorWidget::slotFromLanguageChanged(int index, bool initialize)
{
    const QString lang = d->fromCombobox->itemData(index).toString();
    d->invert->setEnabled(lang != QLatin1String("auto"));
    const QString to = d->toCombobox->itemData(d->toCombobox->currentIndex()).toString();
    d->toCombobox->blockSignals(true);
    d->fillToCombobox(lang);
    d->toCombobox->blockSignals(false);
    const int indexTo = d->toCombobox->findData(to);
    if (indexTo != -1) {
        d->toCombobox->setCurrentIndex(indexTo);
    }
    if (!initialize) {
        slotTranslate();
    }
}

void TranslatorWidget::setTextToTranslate(const QString &text)
{
    d->inputText->setPlainText(text);
    slotTranslate();
}

void TranslatorWidget::slotTranslate()
{
    if (!d->translatorPlugin) {
        qCWarning(TEXTTRANSLATOR_LOG) << " Translator plugin invalid";
        return;
    }
    if (!TextTranslator::NetworkManager::self()->isOnline()) {
        KMessageBox::information(this, i18n("No network connection detected, we cannot translate text."), i18n("No network"));
        return;
    }
    const QString textToTranslate = d->inputText->toPlainText();
    if (textToTranslate.trimmed().isEmpty()) {
        return;
    }

    d->translatorResultTextEdit->clear();

    const QString from = d->fromCombobox->itemData(d->fromCombobox->currentIndex()).toString();
    const QString to = d->toCombobox->itemData(d->toCombobox->currentIndex()).toString();
    d->translate->setEnabled(false);
    d->progressIndicator->show();

    const QString inputText{d->inputText->toPlainText()};
    if (!inputText.isEmpty() && !from.isEmpty() && !to.isEmpty()) {
        d->translatorPlugin->setFrom(from);
        d->translatorPlugin->setTo(to);
        d->translatorPlugin->setInputText(inputText);
        d->translatorPlugin->translate();
    }
}

void TranslatorWidget::slotTranslateDone()
{
    d->translate->setEnabled(true);
    d->progressIndicator->hide();
    d->translatorResultTextEdit->setResultFailed(false);
    d->translatorResultTextEdit->setPlainText(d->translatorPlugin->resultTranslate());
}

void TranslatorWidget::slotTranslateFailed(bool signalFailed, const QString &message)
{
    d->translate->setEnabled(true);
    d->progressIndicator->hide();
    d->translatorResultTextEdit->setResultFailed(signalFailed);
    d->translatorResultTextEdit->clear();
    if (!message.isEmpty()) {
        KMessageBox::error(this, message, i18n("Translate error"));
    }
}

void TranslatorWidget::slotInvertLanguage()
{
    const QString fromLanguage = d->fromCombobox->itemData(d->fromCombobox->currentIndex()).toString();
    // don't invert when fromLanguage == auto
    if (fromLanguage == QLatin1String("auto")) {
        return;
    }

    const QString toLanguage = d->toCombobox->itemData(d->toCombobox->currentIndex()).toString();
    const int indexFrom = d->fromCombobox->findData(toLanguage);
    if (indexFrom != -1) {
        d->fromCombobox->setCurrentIndex(indexFrom);
    }
    const int indexTo = d->toCombobox->findData(fromLanguage);
    if (indexTo != -1) {
        d->toCombobox->setCurrentIndex(indexTo);
    }
    slotTranslate();
}

void TranslatorWidget::setStandalone(bool b)
{
    d->standalone = b;
}

void TranslatorWidget::slotCloseWidget()
{
    if (isHidden()) {
        return;
    }
    d->inputText->clear();
    d->translatorResultTextEdit->clear();
    d->progressIndicator->hide();
    if (d->standalone) {
        hide();
    }
    Q_EMIT toolsWasClosed();
}

bool TranslatorWidget::event(QEvent *e)
{
    // Close the bar when pressing Escape.
    // Not using a QShortcut for this because it could conflict with
    // window-global actions (e.g. Emil Sedgh binds Esc to "close tab").
    // With a shortcut override we can catch this before it gets to kactions.
    if (e->type() == QEvent::ShortcutOverride || e->type() == QEvent::KeyPress) {
        auto kev = static_cast<QKeyEvent *>(e);
        if (kev->key() == Qt::Key_Escape) {
            e->accept();
            slotCloseWidget();
            return true;
        }
    }
    return QWidget::event(e);
}

void TranslatorWidget::slotClear()
{
    d->inputText->clear();
    d->translatorResultTextEdit->clear();
    d->translate->setEnabled(false);
    if (d->translatorPlugin) {
        d->translatorPlugin->clear();
    }
}

void TranslatorWidget::slotDebug()
{
    if (d->translatorPlugin) {
        TranslatorDebugDialog dlg(this);
        dlg.setDebug(d->translatorPlugin->jsonDebug());
        dlg.exec();
    } else {
        qCWarning(TEXTTRANSLATOR_LOG) << " Translator plugin invalid";
    }
}
