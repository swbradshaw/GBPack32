#include "Arduino.h"
#include "EventArgs.h"
#include "Subject.h"
#include "../common.h"
#include <SD.h>

class AudioTheme : public Subject<EventArgs>
{
  public:
    AudioTheme() {
        _theme = 0;
    }

    void loadAudioFiles(fs::FS &fs, const char *dirname, String *audioFiles, int &audioFilesCount) {
        audioFilesCount = 0;
        File root = fs.open(dirname);
        if (!root)
        {
            debugln("Failed to open directory");
            return;
        }
        if (!root.isDirectory())
        {
            debugln("Not a directory");
            return;
        }

        File file = root.openNextFile();
        while (file)
        {
            if (!file.isDirectory()) {
                audioFiles[audioFilesCount] = (String)file.path();
                audioFilesCount++;
            }
            if (audioFilesCount > 50)
            {
                break;
            }
            file = root.openNextFile();
        }
        file.close();
        // sort the quotes alphabetically
        sort(audioFiles, audioFilesCount);
    }

    void init() {
        // read audio clips from SD card
        loadAudioFiles(SD, (getThemeFolder() + "/songs").c_str(), bgSongs, backgroundSongs);
        loadAudioFiles(SD, (getThemeFolder() + "/1").c_str(), sounds, soundClips);
        loadAudioFiles(SD, (getThemeFolder() + "/2").c_str(), quotes, movieQuotes);
        debugln("Background songs loaded: " + String(backgroundSongs));
        debugln("Sound clips loaded: " + String(soundClips));
        debugln("Movie quotes loaded: " + String(movieQuotes));
    }
    void setTheme(int themeNum) {
        _theme = themeNum;
        debugln("Theme set to: " + String(_theme));
        init();
        resetBackground();
    }
    void work() {}

    void advanceBackground() {
        // advance to the next background song
        calculateBackgroundSong();
    }
    void resetBackground() {
        backgroundCounter = -1;
        movieQuoteCounter = 0;
        soundClipCounter = 0;
    }

    String getBackgroundSong() {
        return bgSongs[backgroundCounter];
    }
    String getPostStreamSound() {
        int r = random(10);
        if (r < 1) { // 10% change
            return getThemeFolder() + "/FIRE TAIL V3.mp3";
        } else if (r < 9) { // 80% change
            return getThemeFolder() + "/FIRE TAIL V1.mp3";
        } else { // 10% chance
            return getThemeFolder() + "/FIRE TAIL V2.mp3";
        }
    }

    /***************** Standard Pack Sounds  ****************/
    String getStreamFireSound1() {
        return getThemeFolder() + "/FIRE V1.mp3";
    }
    String getStreamFireWithOverheatSound() {
        return getThemeFolder() + "/FIRE OVERHEAT.mp3";
    }
    String getOverheatSound() {
        return getThemeFolder() + "/Overheat - 7 Beeps.mp3";
    }
    String getVentShortSound() {
        return getThemeFolder() + "/Overheat - Slow.mp3";
    }
    String getVentLongSound() {
        return getThemeFolder() + "/Overheat - Fast.mp3";
    }
    String getStreamSingleFireSound() {
        return getThemeFolder() + "/FIRE SHORT V1.mp3";
    }
    String getSoundClip() {
        String clip = sounds[soundClipCounter];
        soundClipCounter++;
        if (soundClipCounter > soundClips-1) {
            soundClipCounter = 0;
        }
        return clip;
    }
    String getMovieQuote() {
        String clip = quotes[movieQuoteCounter];
        movieQuoteCounter++;
        if (movieQuoteCounter > movieQuotes-1) {
            movieQuoteCounter = 0;
        }
        return clip;
    }
    String getPackPowerOn() {
        return getThemeFolder() + "/POWER UP.mp3";
    }
    String getPackPowerOff() {
        return getThemeFolder() + "/POWER DOWN.mp3";
    }
    String getWandPowerOn(bool packOn) {
        if (packOn) {
            return getThemeFolder() + "/WAND UP-SHORT.mp3";
        } else {
            return getThemeFolder() + "/WAND UP.mp3";
        }

    }
    String getWandPowerOff() {
        return getThemeFolder() + "/WAND DOWN.mp3";
    }
    String getPackBackground() {
         return getThemeFolder() + "/Amb Hum.mp3";
    }
    String getSilence() {
        return "/audio/Silence.mp3";
    }

    void finishedSound(String filename) {
        // pack finsihed playing this sound - determine what this sound is and next steps

    }


    void sort( String *p, int n)
    {
    if( n > 1) {
        for( int i=0; i<n; i++) {
        for( int j=i+1; j<n; j++){
            if( p[j] < p[i]) {
            String q = p[i];
            p[i] = p[j];
            p[j] = q;
            }
        }
        }
    }
    }
  private:

    int _theme = 0;
    long lastRead = 0;


    // max 50 songs / quotes
    String bgSongs[50];
    String quotes[50];
    String sounds[50];

    int backgroundSongs = 0;
    int movieQuotes = 0;
    int soundClips = 0;

    int backgroundCounter = -1;
    int movieQuoteCounter = 0;
    int soundClipCounter = 0;

    String getThemeFolder() {
        switch (_theme) {
            case 0:
                return "/audio/pack-sounds/84";
            case 1:
                return "/audio/pack-sounds/afterlife";
            case 5:
                return "/audio/pack-sounds/kitt";
            case 6:
                return "/audio/pack-sounds/hanukkah";
            case 7:
                return "/audio/pack-sounds/stpatricks";
            case 8:
                return "/audio/pack-sounds/baseball";
            default:
                return "/audio/pack-sounds/84";
        }
    }

    void calculateBackgroundSong() {
        backgroundCounter++;
        if (backgroundCounter > backgroundSongs-1) {
            backgroundCounter = 0;
        }
        debugln("backgroundCounter: " + String(backgroundCounter));
    }

};
