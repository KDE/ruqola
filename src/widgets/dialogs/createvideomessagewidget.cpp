/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagewidget.h"
#include "ruqola_videomessage_debug.h"
#include "ruqolawidgets_debug.h"

#include <KMessageWidget>
#include <QAudioInput>
#include <QCamera>
#include <QCameraDevice>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMediaDevices>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVideoWidget>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDir>
#include <QMediaFormat>
#include <QTemporaryFile>

using namespace Qt::Literals::StringLiterals;
namespace
{
const char myVideoGroupName[] = "Message Video";
}
CreateVideoMessageWidget::CreateVideoMessageWidget(QWidget *parent)
    : QWidget(parent)
    , mVideoWidget(new QVideoWidget(this))
    , mCamera(new QCamera(this))
    , mListCamera(new QComboBox(this))
    , mRecordButton(new QToolButton(this))
    , mPauseButton(new QToolButton(this))
    , mStopButton(new QToolButton(this))
    , mDurationLabel(new QLabel(this))
    , mMessageWidget(new KMessageWidget(this))
    , mMediaDevices(new QMediaDevices(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mMessageWidget->setObjectName(u"mMessageWidget"_s);
    mainLayout->addWidget(mMessageWidget);
    mMessageWidget->setVisible(false);
    mMessageWidget->setCloseButtonVisible(true);
    mMessageWidget->setMessageType(KMessageWidget::Error);
    mMessageWidget->setWordWrap(true);

    mListCamera->setObjectName(u"mListCamera"_s);
    mainLayout->addWidget(mListCamera);

    mVideoWidget->setObjectName(u"mVideoWidget"_s);
    mainLayout->addWidget(mVideoWidget, 1);

    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setObjectName(u"hboxLayout"_s);
    hboxLayout->setContentsMargins({});

    mStopButton->setObjectName(u"mStopButton"_s);
    mStopButton->setIcon(QIcon::fromTheme(u"media-playback-stop"_s));
    hboxLayout->addWidget(mStopButton);
    connect(mStopButton, &QToolButton::clicked, this, &CreateVideoMessageWidget::stop);

    mPauseButton->setObjectName(u"mPauseButton"_s);
    mPauseButton->setIcon(QIcon::fromTheme(u"media-playback-pause"_s));
    hboxLayout->addWidget(mPauseButton);
    connect(mPauseButton, &QToolButton::clicked, this, &CreateVideoMessageWidget::pause);

    mRecordButton->setObjectName(u"mRecordButton"_s);
    mRecordButton->setIcon(QIcon::fromTheme(u"media-record"_s));
    hboxLayout->addWidget(mRecordButton);
    mRecordButton->setEnabled(false);
    connect(mRecordButton, &QToolButton::clicked, this, &CreateVideoMessageWidget::record);

    mDurationLabel->setObjectName(u"mDurationLabel"_s);
    hboxLayout->addWidget(mDurationLabel);
    mainLayout->addLayout(hboxLayout);

    updateCameras();
    if (QMediaDevices::defaultVideoInput().isNull()) {
        mRecordButton->setEnabled(false);
        mPauseButton->setEnabled(false);
    }
    connect(mMediaDevices, &QMediaDevices::videoInputsChanged, this, &CreateVideoMessageWidget::updateCameras);

    connect(mListCamera, &QComboBox::activated, this, [this]() {
        setCamera(mListCamera->itemData(mListCamera->currentIndex()).value<QCameraDevice>());
    });
    connect(mMediaDevices, &QMediaDevices::audioInputsChanged, this, &CreateVideoMessageWidget::updateVideoInputs);
    setCamera(QMediaDevices::defaultVideoInput());
}

CreateVideoMessageWidget::~CreateVideoMessageWidget()
{
    delete mTemporaryFile;
}

void CreateVideoMessageWidget::updateVideoInputs()
{
    mListCamera->clear();
    updateCameras();
}

void CreateVideoMessageWidget::loadSettings()
{
    const KConfigGroup group(KSharedConfig::openConfig(), QLatin1StringView(myVideoGroupName));
    const QByteArray deviceIdentifier = group.readEntry("VideoDevice", QByteArray());
    if (!deviceIdentifier.isEmpty()) {
        for (int i = 0; i < mListCamera->count(); ++i) {
            const QCameraDevice videoDevice = mListCamera->itemData(i).value<QCameraDevice>();
            if (videoDevice.id() == deviceIdentifier) {
                mListCamera->setCurrentIndex(i);
                setCamera(videoDevice);
                break;
            }
        }
    }
}

void CreateVideoMessageWidget::saveSettings()
{
    KConfigGroup group(KSharedConfig::openConfig(), QLatin1StringView(myVideoGroupName));
    const auto device = mListCamera->itemData(mListCamera->currentIndex()).value<QCameraDevice>();
    if (!device.isNull()) {
        const QByteArray deviceIdentifier = device.id();
        group.writeEntry("VideoDevice", deviceIdentifier);
    }
}

QUrl CreateVideoMessageWidget::temporaryFilePath() const
{
    qCDebug(RUQOLA_VIDEOMESSAGE_LOG) << " CreateVideoMessageWidget::temporaryFilePath " << mMediaRecorder->outputLocation() << " actual location "
                                     << mMediaRecorder->actualLocation();
    return mMediaRecorder->actualLocation();
}

void CreateVideoMessageWidget::updateCameras()
{
    const QList<QCameraDevice> availableCameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : availableCameras) {
        mListCamera->addItem(cameraDevice.description(), QVariant::fromValue(cameraDevice));
    }
}

void CreateVideoMessageWidget::setCamera(const QCameraDevice &cameraDevice)
{
    mCamera.reset(new QCamera(cameraDevice));
    mCaptureSession.setCamera(mCamera.data());

    mAudioInput.reset(new QAudioInput);
    mCaptureSession.setAudioInput(mAudioInput.get());

    connect(mCamera.data(), &QCamera::errorOccurred, this, &CreateVideoMessageWidget::displayCameraError);

    if (!mMediaRecorder) {
        mMediaRecorder.reset(new QMediaRecorder);
        mTemporaryFile = new QTemporaryFile(QDir::tempPath() + "/ruqola_XXXXXX"_L1); // TODO fix extension
        mTemporaryFile->setAutoRemove(false);
        if (!mTemporaryFile->open()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to open" << mTemporaryFile->fileName();
        } else {
            //        QMediaFormat format;
            //        format.setFileFormat(QMediaFormat::FileFormat::AVI);
            //        mMediaRecorder->setMediaFormat(format);
            // Define url temporary file.
            mMediaRecorder->setOutputLocation(QUrl::fromLocalFile(mTemporaryFile->fileName()));
            // qDebug() << " store " << mTemporaryFile->fileName();
            mCaptureSession.setRecorder(mMediaRecorder.data());
            connect(mMediaRecorder.data(), &QMediaRecorder::recorderStateChanged, this, &CreateVideoMessageWidget::updateRecorderState);
            connect(mMediaRecorder.data(), &QMediaRecorder::durationChanged, this, &CreateVideoMessageWidget::updateRecordTime);
            connect(mMediaRecorder.data(), &QMediaRecorder::errorChanged, this, &CreateVideoMessageWidget::displayRecorderError);
        }
    }
    mCaptureSession.setVideoOutput(mVideoWidget);
    updateRecorderState(mMediaRecorder->recorderState());

    mCamera->start();
}

void CreateVideoMessageWidget::displayCameraError()
{
    if (mCamera->error() != QCamera::NoError) {
        mMessageWidget->setText(mCamera->errorString());
        mMessageWidget->animatedShow();
    }
}

void CreateVideoMessageWidget::updateRecorderState(QMediaRecorder::RecorderState state)
{
    qCDebug(RUQOLA_VIDEOMESSAGE_LOG) << " CreateVideoMessageWidget::updateRecorderState " << state;
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

void CreateVideoMessageWidget::displayRecorderError()
{
    if (mMediaRecorder->error() != QMediaRecorder::NoError) {
        mMessageWidget->setText(mMediaRecorder->errorString());
        mMessageWidget->animatedShow();
    }
}

void CreateVideoMessageWidget::record()
{
    qCDebug(RUQOLA_VIDEOMESSAGE_LOG) << " CreateVideoMessageWidget::record ";
    mMediaRecorder->record();
    updateRecordTime();
}

void CreateVideoMessageWidget::pause()
{
    qCDebug(RUQOLA_VIDEOMESSAGE_LOG) << " CreateVideoMessageWidget::pause ";
    mMediaRecorder->pause();
}

void CreateVideoMessageWidget::stop()
{
    qCDebug(RUQOLA_VIDEOMESSAGE_LOG) << " CreateVideoMessageWidget::stop ";
    mMediaRecorder->stop();
    Q_EMIT recordDone();
}

void CreateVideoMessageWidget::updateRecordTime()
{
    const QString str = i18n("Recorded %1 sec", mMediaRecorder->duration() / 1000);
    mDurationLabel->setText(str);
}

#include "moc_createvideomessagewidget.cpp"
