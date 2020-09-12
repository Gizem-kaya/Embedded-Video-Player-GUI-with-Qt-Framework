# Embedded Video Player GUI with Qt Framework

## The project to write a video playback interface from the memory of an existing electronic board or an URL.


![Screenshot_1](https://user-images.githubusercontent.com/32525636/66859290-ab9dfe80-ef93-11e9-82b5-17ac419523f1.png)

Since the whole project is confidential, I didn't hesitate to show this part that I was working on.

The development stages of the project are as follows

Error types and their solutions:

	For local video files:

		Cannot connect to server socket err = No such file or directory
		Cannot connect to server request channel
		jack server is not running or cannot be started
		Internal data flow error.
	
	For videos from Urls:
	
		Cannot connect to server socket err = No such file or directory
		Cannot connect to server request channel
		jack server is not running or cannot be started
		Internal data stream error.
		
	
	As a result of research :

- GStreamer package setup has to be made.
```		
  sudo apt-get update
  sudo apt-get upgrade
  sudo apt-get install libgstreamer1.0-0
  sudo apt-get install gstreamer1.0-plugins-base gstreamer1.0-plugins-good
  sudo apt-get install gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly
  sudo apt-get install gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools
```		
	conclusion : to install gstreamer 1.0, using the backports for Precise from the gstreamer developers ppa is needed.

-  Installing all the base packages and plugins that we require is needed.
```		
sudo apt-get install ubuntu-restricted-extras
sudo add-apt-repository ppa:gstreamer-developers/ppa 
```

		Error : Cannot add PPA: 'ppa:gstreamer-developers/ppa'.
			Please check that the PPA name or format is correct.
						
to solve this problem :
```
sudo apt-get install --reinstall ca-certificates 
```

	conclusion : problems could not be solved and other solutions started to be searched.
						
- Other version of GStreamer package setup.

apt-get install gstreamer1.0-tools

- GStreamer test :
```
gst-inspect-1.0 fakesrc
gst-launch-1.0 -v fakesrc silent=false num-buffers=3 ! fakesink silent=false
gst-launch-1.0 videotestsrc ! videoconvert ! autovideosink
```

		conclusion : GStreamer setup is successful but did not resolve an error.
						
						
	- Solutions for the given socket and Jack error :		
pkill -9 jackdbus

	- For starting jack:
jackd -r -d alsa -r 44100	
		Error : AL lib: UpdateDeviceParams: Failed to set 44100hz, got 48000hz instead	
					
- A much simpler way it to start jackd with the no soundcard at the target-rate and then add sound cards using zita-ajbridge or alsa_in/out.	
jackd -d dummy -r 44100				
	
conclusion : none of them helped to correct the error, other solutions were searched.
							
- Attempts to switch to VLC Player instead of Media Player.
	- Playing the video from the terminal. 	
 gst-launch-1.0 playbin uri=file:///home/abdullah/Downloads/small.avi
	conclusion : Video playback succeeded. GST-Launch-1.0 is running.						
- Installing VLC player libraries.
sudo apt-get install libvlc-dev

- Installing VLC player plugins.
sudo apt-get install vlc
				
	Error : configure: error: Package requirements (xcb-shm) were not met:
		No package 'xcb-shm' found
		Consider adjusting the PKG_CONFIG_PATH environment variable if you
		installed software in a non-standard prefix.
		Alternatively, you may set the environment variables XCB_SHM_CFLAGS
			and XCB_SHM_LIBS to avoid the need to call pkg-config.
		See the pkg-config man page for more details.
											
	conclusion : Still not available VLC player in QtCreator.	
		
	- Installing required libraries.

apt-get install libdca-dev libflac-dev libmpeg2-4-dev libspeexdsp-dev libschroedinger-dev libdirac-dev libfluidsynth-dev libkate-dev liboggkate-dev libxcb-shm0-dev
```				
sudo apt-get install libxcb-composite0-dev
```
			Error: E: Could not open lock file /var/lib/dpkg/lock - open (13: Permission denied)
				E: Unable to lock the administration directory (/var/lib/dpkg/), are you root?
			
```			
sudo apt-get install libxcb-shm0-dev
```
		configure: error: Package requirements (xcb-xv) were not met:
		No package 'xcb-xv' found
		Consider adjusting the PKG_CONFIG_PATH environment variable if you
				installed software in a non-standard prefix.
		Alternatively, you may set the environment variables XCB_SHM_CFLAGS
			and XCB_SHM_LIBS to avoid the need to call pkg-config.
			See the pkg-config man page for more details.
					
					
sudo apt-get install libfribidi-dev					
						
						
./configure --disable-lua --disable-mad --disable-avcodec --disable-postproc --disable-a52
				
			Error : configure: error: The skins2 module depends on a the Qt4 development package. Without it you won't be able to open any dialog box from the interface,
			which makes the skins2 interface rather useless. Install the Qt4 development package or alternatively you can also configure with: --disable-qt4 --disable-skins2.
```				
./configure --disable-lua --disable-mad --disable-avcodec --disable-postproc --disable-a52 --disable-qt4 --disable-skins2
```

	Error : configure: error: X11 video outputs need a window provider (Qt4, Skins2, Hildon or xcb-utils), but none were found. Please install xcb-keysyms.
```					
sudo apt-get install libxcb-keysyms1-dev
```
				
	conclusion : The project could not be run with VLC player as a result of operations.

				
	- Opened a new project, running a simple video player.
		
```
		
	#include "videoplayer.h"
	#include <QtWidgets>
	#include <QVideoWidget>

	VideoPlayer::VideoPlayer(QWidget *parent)
		: QWidget(parent)
	{
		m_mediaPlayer = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
			QVideoWidget *videoWidget = new QVideoWidget;

		QAbstractButton *openButton = new QPushButton(tr("Open..."));
			connect(openButton, &QAbstractButton::clicked, this, &VideoPlayer::openFile);

		m_playButton = new QPushButton;
		m_playButton->setEnabled(false);
			m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

		connect(m_playButton, &QAbstractButton::clicked,
					this, &VideoPlayer::play);
		m_positionSlider = new QSlider(Qt::Horizontal);
		m_positionSlider->setRange(0, 0);
		connect(m_positionSlider, &QAbstractSlider::sliderMoved,
				this, &VideoPlayer::setPosition);
		m_errorLabel = new QLabel;
		m_errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
		QBoxLayout *controlLayout = new QHBoxLayout;
		controlLayout->setMargin(0);
		controlLayout->addWidget(openButton);
		controlLayout->addWidget(m_playButton);
		controlLayout->addWidget(m_positionSlider);

		QBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(videoWidget);
		layout->addLayout(controlLayout);
		layout->addWidget(m_errorLabel);
		setLayout(layout);

		m_mediaPlayer->setVideoOutput(videoWidget);
		connect(m_mediaPlayer, &QMediaPlayer::stateChanged,
					this, &VideoPlayer::mediaStateChanged);
		connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
		connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &VideoPlayer::durationChanged);
		connect(m_mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
				this, &VideoPlayer::handleError);
	}		
```
	conclusion :  it gives the same error. So, checking this part of the code is needed.
			
			
	- Opened another project, running a different simple video player.
		
```	
		
		QGraphicsView * graphicsView = new QGraphicsView;
		QMediaPlayer * player = new QMediaPlayer;

		QGraphicsScene * scene = new QGraphicsScene;
		graphicsView->setScene(scene);

		QGraphicsVideoItem *item = new QGraphicsVideoItem;
		player->setVideoOutput(item);
		graphicsView->scene()->addItem(item);
		graphicsView->show();

			player->setMedia(QUrl::fromLocalFile("/home/abdullah/Downloads/small.avi"));
			player->setMedia(QUrl("http://techslides.com/demos/sample-videos/small.flv"));
			player->play();
			
```
	conclusion : this project worked without error. Differences were examined. 

```

	QGraphicsView * graphicsView = new QGraphicsView;					
	QVideoWidget *videoWidget = new QVideoWidget;
	QMediaPlayer * player = new QMediaPlayer;		

```	
				
	difference stood out. The original project is organized according to the working project. This part of code enabled us to play the video in a new window. If we wanted to play it in gui, it gave "internal data flow error" error again.
				
				
	Therefore the code has been changed to

```			
		QGraphicsView * graphicsView = new QGraphicsView;
		QGraphicsScene * scene = new QGraphicsScene();

		graphicsView->setScene(scene);

		QGraphicsVideoItem *item = new QGraphicsVideoItem;
		graphicsView->scene()->addItem(item);
		layout->addWidget(graphicsView);

```
So that the video was played in the GUI.
				
		- "Open...", "play" and "pause" button was broken.
			
			The buttons had some problems. The problems in
																					VideoPlayer::play(),															VideoPlayer::openFile(),														VideoPlayer::activate(int mode) fixed.
				
-  After the video can be played in gui without error, the image of gui is as follows :


-  The "Video from Stream" button was used to play video from a link which is embedded in the code. 
	In order to improve this event, it is planned to display a keyboard when the "Video from Stream" button is pressed and play the video by entering the desired link on this keyboard.	
			
			
To accomplish this, several more files had to be added to the project. 
			
-  keyboard.cpp
			-  keyboard.h
			-  keyboard.ui
			-  ui_keyboard.h
			
	These files were written manually, ie no qt function was used. 
	Added files enabled a virtual keyboard to be displayed in a new window.
		
After opening the keyboard in a new window, the image of gui is as follows :
		
				

 -    After that, the URL entered from the keyboard should be read and sent to video player. In order to do this, first of all, when “Video from Stream Test” button was pressed, instead of playing the video, pressing the enter key on the keyboard had to enable playback. To ensure that the following field has been added to the keyboard’s header :

keyboard.h
```
public:
    VideoPlayer* videoPlayer;
```
After that, the following line has been added to the “on_enterButton_clicked()” in the keyboard.cpp :

```
this->videoPlayer->activate(playstream, outputText);
```

and the signals to which this button was previously connected and calling the activate function are interrupted.


-  When all operations were completed, the video could be played with the link entered from the keyboard as shown in the screenshot below :




As soon as you press “Enter”, the Keyboard window closes and the video starts playing.




- Finally, the desired video could be played in GUI in a small size. To be corrected to fit the GUI fully :
```
QSize size = graphicsView->size();
item->setSize(size);

```
- The size of the QGraphicsView object was kept in a variable. Then it was assigned to the size of the QGraphicsVideoItem object.
