#pragma once
#include <QWidget>
#include <QMediaPlayer>

class QPushButton;
class QSlider;
class QLabel;
class QVBoxLayout;

class AudioPlayer : public QWidget
{
  Q_OBJECT
protected:
  void paintEvent(QPaintEvent *);
private:
  QMediaPlayer *m_pmp;
  QVBoxLayout *m_pvbxMainLayout;
  QPushButton *m_pcmdOpen;
  QSlider *m_psldVolume;
  QPushButton *m_pcmdPlay;
  QPushButton *m_pcmdStop;
  QSlider *m_psldPosition;
  QLabel *m_plblCurrent;
  QLabel *m_plblRemain;
  QString msecsToString(qint64 n);

public:
  AudioPlayer(QWidget *pwgt = 0);
private slots:
  void slotOpen();
  void slotPlay();
  void slotSetSliderPosition(qint64);
  void slotSetMediaPosition(int);
  void slotSetDuration(qint64);
  void slotStatusChanged(QMediaPlayer::State);
};