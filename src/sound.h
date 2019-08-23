#ifndef SOUND_H
#define SOUND_H

#include <QString>
#include <QSoundEffect>

class Sound
{
public:
    Sound();
    enum{
        SING = 0,
        DANCE,
        RAP,
        BASKETBALL,
        MUSIC
    };
    void play(int num){
        switch (num) {
        case SING:
            sing.play();
            break;
        case DANCE:
            dance.play();
            break;
        case RAP:
            rap.play();
            break;
        case BASKETBALL:
            basketball.play();
            break;
        case MUSIC:
            music.play();
            break;
        default:
            break;
        }
    }
private:
    QString sing_file, dance_file, rap_file, basketball_file, music_file;
    QSoundEffect sing, dance, rap, basketball, music;
};

#endif // SOUND_H
