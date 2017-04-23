#include <QtWidgets>
#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(QWidget *pwgt) : QWidget(pwgt)
{
    m_pcmdOpen = new QPushButton("&Open");
    m_pcmdOpen->setStyleSheet("QPushButton { max-width:100px; max-height: 50px; }");

    m_psldVolume = new QSlider;
    m_psldVolume->setStyleSheet("QSlider { max-height:100px }");

    m_pcmdPlay = new QPushButton;
    m_pcmdPlay->setStyleSheet("QPushButton { max-width:50px; max-height: 50px; }");

    m_pcmdStop = new QPushButton;
    m_pcmdStop->setStyleSheet("QPushButton { max-width:50px; max-height: 50px; }");

    m_psldPosition = new QSlider;
    m_plblCurrent = new QLabel(msecsToString(0));
    m_plblRemain = new QLabel(msecsToString(0));
    m_pmp = new QMediaPlayer;

    m_pcmdPlay->setEnabled(false);
    m_pcmdPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    m_pcmdStop->setEnabled(false);
    m_pcmdStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

    m_psldPosition->setRange(0, 0);
    m_psldPosition->setOrientation(Qt::Horizontal);

    m_psldVolume->setRange(0, 100);
    int nDefaultVolume = 50;
    m_pmp->setVolume(nDefaultVolume);
    m_psldVolume->setValue(nDefaultVolume);

    connect(m_pcmdOpen, SIGNAL(clicked()), SLOT(slotOpen()));
    connect(m_pcmdPlay, SIGNAL(clicked()), SLOT(slotPlay()));
    connect(m_pcmdStop, SIGNAL(clicked()), m_pmp, SLOT(stop()));
    connect(m_psldVolume, SIGNAL(valueChanged(int)), m_pmp, SLOT(setVolume(int)));
    connect(m_psldPosition, SIGNAL(sliderMoved(int)), SLOT(slotSetMediaPosition(int)));
    connect(m_pmp, SIGNAL(positionChanged(qint64)), SLOT(slotSetSliderPosition(qint64)));
    connect(m_pmp, SIGNAL(durationChanged(qint64)), SLOT(slotSetDuration(qint64)));
    connect(m_pmp, SIGNAL(stateChanged(QMediaPlayer::State)), SLOT(slotStatusChanged(QMediaPlayer::State)));
    QHBoxLayout *phbxPlayControls = new QHBoxLayout;
    phbxPlayControls->addWidget(m_pcmdOpen);
    phbxPlayControls->addWidget(m_pcmdPlay);
    phbxPlayControls->addWidget(m_pcmdStop);
    phbxPlayControls->addWidget(m_psldVolume);

    QHBoxLayout *phbxTimeControls = new QHBoxLayout;
    phbxTimeControls->addWidget(m_plblCurrent);
    phbxTimeControls->addWidget(m_psldPosition);
    phbxTimeControls->addWidget(m_plblRemain);

    m_pvbxMainLayout = new QVBoxLayout;
    m_pvbxMainLayout->addLayout(phbxPlayControls);
    m_pvbxMainLayout->addLayout(phbxTimeControls);

    setLayout(m_pvbxMainLayout);
}

void AudioPlayer::slotOpen()
{
    QString strFile = QFileDialog::getOpenFileName(this, "Open File");
    if (!strFile.isEmpty())
    {
        m_pmp->setMedia(QUrl::fromLocalFile(strFile));
        m_pcmdPlay->setEnabled(true);
        m_pcmdStop->setEnabled(true);
    }
}

void AudioPlayer::slotPlay()
{
    switch (m_pmp->state())
    {
    case QMediaPlayer::PlayingState:
        m_pmp->pause();
        break;
    default:
        m_pmp->play();
        break;
    }
}

void AudioPlayer::slotStatusChanged(QMediaPlayer::State state)
{
    switch (state)
    {
    case QMediaPlayer::PlayingState:
        m_pcmdPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        m_pcmdPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void AudioPlayer::slotSetMediaPosition(int n)
{
    m_pmp->setPosition(n);
}

QString AudioPlayer::msecsToString(qint64 n)
{
    int nHours = (n / (60 * 60 * 1000));
    int nMinutes = ((n % (60 * 60 * 1000)) / (600 * 1000));
    int nSeconds = ((n % (60 * 1000)) / 1000);

    return QTime(nHours, nMinutes, nSeconds).toString("hh:mm:ss");
}

void AudioPlayer::slotSetDuration(qint64 n)
{
    m_psldPosition->setRange(0, n);
    m_plblCurrent->setText(msecsToString(0));
    m_plblRemain->setText(msecsToString(n));
}

void AudioPlayer::slotSetSliderPosition(qint64 n)
{
    m_psldPosition->setValue(n);
    m_plblCurrent->setText(msecsToString(n));
    int nDuration = m_psldPosition->maximum();
    m_plblRemain->setText(msecsToString(nDuration - n));
}

void AudioPlayer::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}