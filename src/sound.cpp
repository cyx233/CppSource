#include <QCoreApplication>
#include "sound.h"

Sound::Sound()
{
    QString dir = QCoreApplication::applicationDirPath();

    sing_file = QString(dir + "/sound/sing.wav");
    dance_file = QString(dir + "/sound/dance.wav");
    rap_file = QString(dir + "/sound/rap.wav");
    basketball_file = QString(dir + "/sound/basketball.wav");
    music_file = QString(dir + "/sound/music.wav");

    sing.setSource(QUrl::fromLocalFile(sing_file));
    dance.setSource(QUrl::fromLocalFile(dance_file));
    rap.setSource(QUrl::fromLocalFile(rap_file));
    basketball.setSource(QUrl::fromLocalFile(basketball_file));
    music.setSource(QUrl::fromLocalFile(music_file));

}
