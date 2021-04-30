#include <SFML/Audio.hpp>
#include <string>

using namespace std;
using namespace sf;

class SoundManager                            //CLASS TO PLAY SOUNDS FOR VARIOUS EVENTS
{
private:
    sf::SoundBuffer launch_buff;
    sf::Sound sound;
public:
    SoundManager(string);
    void playLaunch();

};
SoundManager::SoundManager(string hear)       //constructor to load the sound from memory
{

    launch_buff.loadFromFile(hear);

}
void SoundManager::playLaunch()              //member function to play the sound
{

    sound.setBuffer(launch_buff);
    sound.play();
}
