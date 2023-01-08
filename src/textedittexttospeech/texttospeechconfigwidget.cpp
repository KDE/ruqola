/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechconfigwidget.h"
#include "abstracttexttospeechconfiginterface.h"
#include "texttospeechconfiginterface.h"
#include "texttospeechlanguagecombobox.h"
#include "texttospeechsliderwidget.h"
#include "texttospeechutil.h"
#include <KLocalizedString>

#include <KConfig>
#include <KConfigGroup>
#include <QComboBox>
#include <QDebug>
#include <QFormLayout>
#include <QPushButton>
#include <QSlider>
#include <QTimer>

using namespace TextEditTextToSpeech;
TextToSpeechConfigWidget::TextToSpeechConfigWidget(QWidget *parent)
    : QWidget(parent)
    , mVolume(new TextToSpeechSliderWidget(QStringLiteral("%1 %"), this))
    , mRate(new TextToSpeechSliderWidget(QStringLiteral("%1"), this))
    , mPitch(new TextToSpeechSliderWidget(QStringLiteral("%1"), this))
    , mLanguage(new TextEditTextToSpeech::TextToSpeechLanguageComboBox(this))
    , mAbstractTextToSpeechConfigInterface(new TextToSpeechConfigInterface(this))
    , mAvailableEngine(new QComboBox(this))
    , mVoice(new QComboBox(this))
    , mTestButton(new QPushButton(QIcon::fromTheme(QStringLiteral("player-volume")), i18n("Test"), this))
{
    auto layout = new QFormLayout(this);
    mVolume->setObjectName(QStringLiteral("volume"));
    mVolume->setRange(0, 100);
    connect(mVolume, &TextToSpeechSliderWidget::valueChanged, this, &TextToSpeechConfigWidget::valueChanged);

    layout->addRow(i18n("Volume:"), mVolume);

    mRate->setObjectName(QStringLiteral("rate"));
    mRate->setRange(-100, 100);
    layout->addRow(i18n("Rate:"), mRate);
    connect(mRate, &TextToSpeechSliderWidget::valueChanged, this, &TextToSpeechConfigWidget::valueChanged);

    mPitch->setRange(-100, 100);
    connect(mPitch, &TextToSpeechSliderWidget::valueChanged, this, &TextToSpeechConfigWidget::valueChanged);
    mPitch->setObjectName(QStringLiteral("pitch"));
    layout->addRow(i18n("Pitch:"), mPitch);

    mAvailableEngine->setObjectName(QStringLiteral("engine"));
    layout->addRow(i18n("Engine:"), mAvailableEngine);
    connect(mAvailableEngine, &QComboBox::currentIndexChanged, this, &TextToSpeechConfigWidget::slotAvailableEngineChanged);

    mLanguage->setObjectName(QStringLiteral("language"));
    layout->addRow(i18n("Language:"), mLanguage);
    connect(mLanguage, &QComboBox::currentIndexChanged, this, &TextToSpeechConfigWidget::valueChanged);

    mVoice->setObjectName(QStringLiteral("voice"));
    layout->addRow(i18n("Voice:"), mVoice);
    connect(mVoice, &QComboBox::currentIndexChanged, this, &TextToSpeechConfigWidget::valueChanged);

    mTestButton->setObjectName(QStringLiteral("mTestButton"));
    layout->addWidget(mTestButton);
    connect(mTestButton, &QPushButton::clicked, this, &TextToSpeechConfigWidget::slotTestTextToSpeech);

    QTimer::singleShot(0, this, &TextToSpeechConfigWidget::slotUpdateSettings);
}

TextToSpeechConfigWidget::~TextToSpeechConfigWidget() = default;

void TextToSpeechConfigWidget::slotAvailableEngineChanged()
{
    slotEngineChanged();
    slotLanguageChanged();
    valueChanged();
}

void TextToSpeechConfigWidget::valueChanged()
{
    Q_EMIT configChanged(true);
}

void TextToSpeechConfigWidget::updateLocale()
{
    KConfig config(TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigFileName());
    const KConfigGroup grp = config.group(TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigGroupName());
    const QString localeName = grp.readEntry("localeName");
    if (localeName.isEmpty()) {
        return;
    }
    mLanguage->selectLocaleName(localeName);
}

void TextToSpeechConfigWidget::readConfig()
{
    KConfig config(TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigFileName());
    const KConfigGroup grp = config.group(TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigGroupName());
    const auto rate = grp.readEntry("rate", 0);
    mRate->setValue(rate);
    const auto pitch = grp.readEntry("pitch", 0);
    mPitch->setValue(pitch);
    mVolume->setValue(grp.readEntry("volume", 50));
}

void TextToSpeechConfigWidget::writeConfig()
{
    KConfig config(TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigFileName());
    KConfigGroup grp = config.group(TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigGroupName());
    grp.writeEntry("volume", mVolume->value());
    grp.writeEntry("rate", mRate->value());
    grp.writeEntry("pitch", mPitch->value());
    grp.writeEntry("localeName", mLanguage->currentData().toLocale().name());
    grp.writeEntry("engine", mAvailableEngine->currentData().toString());
    grp.writeEntry("voice", mVoice->currentData().toString());
}

void TextToSpeechConfigWidget::slotLocalesAndVoices()
{
    updateAvailableLocales();
    updateAvailableVoices();
}

void TextToSpeechConfigWidget::slotUpdateSettings()
{
    updateAvailableEngine();
    slotLocalesAndVoices();
}

void TextToSpeechConfigWidget::setTextToSpeechConfigInterface(AbstractTextToSpeechConfigInterface *interface)
{
    delete mAbstractTextToSpeechConfigInterface;
    mAbstractTextToSpeechConfigInterface = interface;
    slotLocalesAndVoices();
}

void TextToSpeechConfigWidget::restoreDefaults()
{
    mRate->setValue(0);
    mPitch->setValue(0);
    mVolume->setValue(50);

    // TODO
}

void TextToSpeechConfigWidget::slotTestTextToSpeech()
{
    TextToSpeechConfigInterface::EngineSettings settings;
    settings.rate = mRate->value();
    settings.pitch = mPitch->value();
    settings.volume = mVolume->value();
    settings.localeName = mLanguage->currentData().toLocale().name();
    settings.voice = mVoice->currentData().toString();
    mAbstractTextToSpeechConfigInterface->testEngine(settings);
}

void TextToSpeechConfigWidget::updateAvailableEngine()
{
    mAvailableEngine->clear();
    const QStringList lst = mAbstractTextToSpeechConfigInterface->availableEngines();
    for (const QString &engine : lst) {
        mAvailableEngine->addItem(engine, engine);
    }
    mAvailableEngine->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    updateEngine();
}

void TextToSpeechConfigWidget::updateAvailableVoices()
{
    mVoice->clear();
    const QStringList lst = mAbstractTextToSpeechConfigInterface->availableVoices();
    for (const QString &voice : lst) {
        mVoice->addItem(voice, voice);
    }
    mVoice->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    updateVoice();
}

void TextToSpeechConfigWidget::updateVoice()
{
    KConfig config(TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigFileName());
    const KConfigGroup grp = config.group(TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigGroupName());
    const QString voice = grp.readEntry("voice");
    int index = mVoice->findData(voice);
    if (index == -1) {
        index = 0;
    }
    mVoice->setCurrentIndex(index);
}

void TextToSpeechConfigWidget::updateEngine()
{
    KConfig config(TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigFileName());
    const KConfigGroup grp = config.group(TextEditTextToSpeech::TextToSpeechUtil::textToSpeechConfigGroupName());
    const QString engineName = grp.readEntry("engine");
    int index = mAvailableEngine->findData(engineName);
    if (index == -1) {
        index = 0;
    }
    mAvailableEngine->setCurrentIndex(index);
}

void TextToSpeechConfigWidget::updateAvailableLocales()
{
    mLanguage->clear();
    const QVector<QLocale> locales = mAbstractTextToSpeechConfigInterface->availableLocales();
    const QLocale current = mAbstractTextToSpeechConfigInterface->locale();
    mLanguage->updateAvailableLocales(locales, current);
    updateLocale();
}

void TextToSpeechConfigWidget::slotEngineChanged()
{
    mAbstractTextToSpeechConfigInterface->setEngine(mAvailableEngine->currentData().toString());
}

void TextToSpeechConfigWidget::slotLanguageChanged()
{
    // QLocale locale = mLanguage->currentData().value<QLocale>();
}
