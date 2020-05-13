#ifndef GUIVIDEOPLAYER_H
#define GUIVIDEOPLAYER_H

#include <QMediaPlayer>
#include <QWidget>
#include "keyboard/keyboard.h"

class Keyboard;
class QAbstractButton;
class QSlider;
class QLabel;
class QUrl;

enum modes{
    playfile,
    playstream,
};

class VideoPlayer : public QWidget
{
    Q_OBJECT
public:
    VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

    void setUrl(QUrl &url);

public slots:
    void openFile();
    void play();
    void activate(int mode, QString url);
    void run_keyboard_lineEdit();


private slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void handleError();


private:
    QMediaPlayer* m_mediaPlayer;
    QAbstractButton *m_playButton;
    QSlider *m_positionSlider;
    QLabel *m_errorLabel;
    QVideoWidget *videoWidget;
    Keyboard *lineEditkeyboard;

};


#endif
