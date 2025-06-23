/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagewidget.h"
#include "ruqolawidgets_debug.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KMessageWidget>
#include <KSharedConfig>
#include <QAudioDevice>
#include <QAudioInput>
#include <QComboBox>
#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QMediaDevices>
#include <QTemporaryFile>
#include <QToolButton>

using namespace Qt::Literals::StringLiterals;
namespace
{
const char mySoundGroupName[] = "Message Sound";
}

CreateSoundMessageWidget::CreateSoundMessageWidget(QWidget *parent)
    : QWidget{parent}
    , mRecordButton(new QToolButton(this))
    , mPauseButton(new QToolButton(this))
    , mStopButton(new QToolButton(this))
    , mLabelDuration(new QLabel(this))
    , mAudioRecorder(new QMediaRecorder(this))
    , mDeviceComboBox(new QComboBox(this))
    , mMessageWidget(new KMessageWidget(this))
    , mMediaDevices(new QMediaDevices(this))
{
    mCaptureSession.setRecorder(mAudioRecorder);
    mCaptureSession.setAudioInput(new QAudioInput(this));

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mMessageWidget->setObjectName(u"mMessageWidget"_s);
    mainLayout->addWidget(mMessageWidget);
    mMessageWidget->setVisible(false);
    mMessageWidget->setCloseButtonVisible(true);
    mMessageWidget->setMessageType(KMessageWidget::Error);
    mMessageWidget->setWordWrap(true);

    mDeviceComboBox->setObjectName(u"mDeviceComboBox"_s);
    mainLayout->addWidget(mDeviceComboBox);

    auto soundWidgetLayout = new QHBoxLayout;
    soundWidgetLayout->setObjectName(u"mainLayout"_s);
    mainLayout->addLayout(soundWidgetLayout);

    mStopButton->setObjectName(u"mStopButton"_s);
    mStopButton->setIcon(QIcon::fromTheme(u"media-playback-stop"_s));
    soundWidgetLayout->addWidget(mStopButton);
    connect(mStopButton, &QToolButton::clicked, this, &CreateSoundMessageWidget::stop);

    mPauseButton->setObjectName(u"mPauseButton"_s);
    mPauseButton->setIcon(QIcon::fromTheme(u"media-playback-pause"_s));
    soundWidgetLayout->addWidget(mPauseButton);
    connect(mPauseButton, &QToolButton::clicked, this, &CreateSoundMessageWidget::pause);

    mRecordButton->setObjectName(u"mRecordButton"_s);
    mRecordButton->setIcon(QIcon::fromTheme(u"media-record"_s));
    soundWidgetLayout->addWidget(mRecordButton);
    connect(mRecordButton, &QToolButton::clicked, this, &CreateSoundMessageWidget::record);

    mLabelDuration->setObjectName(u"mLabelDuration"_s);
    mLabelDuration->setTextFormat(Qt::PlainText);
    soundWidgetLayout->addWidget(mLabelDuration);

    connect(mAudioRecorder, &QMediaRecorder::durationChanged, this, &CreateSoundMessageWidget::updateRecordTime);
    connect(mAudioRecorder, &QMediaRecorder::recorderStateChanged, this, &CreateSoundMessageWidget::updateRecorderState);
    connect(mAudioRecorder, &QMediaRecorder::errorChanged, this, &CreateSoundMessageWidget::displayRecorderError);
    connect(mMediaDevices, &QMediaDevices::audioInputsChanged, this, &CreateSoundMessageWidget::updateAudioInputs);
    initializeInput();
    updateRecorderState(mAudioRecorder->recorderState());
}

CreateSoundMessageWidget::~CreateSoundMessageWidget()
{
    delete mTemporaryFile;
}

void CreateSoundMessageWidget::updateAudioInputs()
{
    mDeviceComboBox->clear();
    mDeviceComboBox->addItem(i18n("Default"), QVariant(QString()));
    for (const auto &device : QMediaDevices::audioInputs()) {
        const auto name = device.description();
        mDeviceComboBox->addItem(name, QVariant::fromValue(device));
    }
}

void CreateSoundMessageWidget::loadSettings()
{
    KConfigGroup group(KSharedConfig::openConfig(), QLatin1StringView(mySoundGroupName));
    const QByteArray deviceIdentifier = group.readEntry("SoundDevice", QByteArray());
    if (!deviceIdentifier.isEmpty()) {
        for (int i = 0; i < mDeviceComboBox->count(); ++i) {
            const QAudioDevice audioDevice = mDeviceComboBox->itemData(i).value<QAudioDevice>();
            if (audioDevice.id() == deviceIdentifier) {
                mDeviceComboBox->setCurrentIndex(i);
                break;
            }
        }
    }
}

void CreateSoundMessageWidget::saveSettings()
{
    KConfigGroup group(KSharedConfig::openConfig(), QLatin1StringView(mySoundGroupName));
    const auto device = mDeviceComboBox->itemData(mDeviceComboBox->currentIndex()).value<QAudioDevice>();
    if (!device.isNull()) {
        const QByteArray deviceIdentifier = device.id();
        group.writeEntry("SoundDevice", deviceIdentifier);
    }
}

void CreateSoundMessageWidget::displayRecorderError()
{
    if (mAudioRecorder->error() != QMediaRecorder::NoError) {
        mMessageWidget->setText(mAudioRecorder->errorString());
        mMessageWidget->animatedShow();
    }
}

void CreateSoundMessageWidget::initializeInput()
{
    mDeviceComboBox->addItem(i18n("Default"), QVariant(QString()));
    for (const auto &device : QMediaDevices::audioInputs()) {
        const auto name = device.description();
        mDeviceComboBox->addItem(name, QVariant::fromValue(device));
    }

    mTemporaryFile = new QTemporaryFile(QDir::tempPath() + "/ruqola_XXXXXX"_L1); // TODO fix extension
    mTemporaryFile->setAutoRemove(false);
    if (!mTemporaryFile->open()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to open" << mTemporaryFile->fileName();
    } else {
        //        QMediaFormat format;
        //        format.setFileFormat(QMediaFormat::FileFormat::AVI);
        //        mMediaRecorder->setMediaFormat(format);
        // Define url temporary file.
        // qDebug() << " store " << mTemporaryFile->fileName();
        mAudioRecorder->setOutputLocation(QUrl::fromLocalFile(mTemporaryFile->fileName()));
    }
}

void CreateSoundMessageWidget::updateRecordTime(qint64 duration)
{
    const QString str = i18n("Recorded %1 sec", duration / 1000);
    mLabelDuration->setText(str);
}

QUrl CreateSoundMessageWidget::temporaryFilePath() const
{
    // qDebug() << "output location" << mMediaRecorder->outputLocation() << " dd " << mMediaRecorder->actualLocation();
    return mAudioRecorder->actualLocation();
}

void CreateSoundMessageWidget::stop()
{
    mAudioRecorder->stop();
    Q_EMIT recordDone();
}

void CreateSoundMessageWidget::record()
{
    mCaptureSession.audioInput()->setDevice(mDeviceComboBox->itemData(mDeviceComboBox->currentIndex()).value<QAudioDevice>());
    mAudioRecorder->record();
}

void CreateSoundMessageWidget::pause()
{
    mAudioRecorder->pause();
}

void CreateSoundMessageWidget::updateRecorderState(QMediaRecorder::RecorderState state)
{
    switch (state) {
    case QMediaRecorder::StoppedState:
        mRecordButton->setEnabled(true);
        mPauseButton->setEnabled(true);
        mStopButton->setEnabled(false);
        break;
    case QMediaRecorder::PausedState:
        mRecordButton->setEnabled(true);
        mPauseButton->setEnabled(false);
        mStopButton->setEnabled(true);
        break;
    case QMediaRecorder::RecordingState:
        mRecordButton->setEnabled(false);
        mPauseButton->setEnabled(true);
        mStopButton->setEnabled(true);
        break;
    }
}

#include "moc_createsoundmessagewidget.cpp"
