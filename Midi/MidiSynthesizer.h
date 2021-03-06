#ifndef MIDISYNTHESIZER_H
#define MIDISYNTHESIZER_H

/*
    Midi stream custom to use 32 channel

        channel 0  - 15 is default intruments
        channel 16 - 31 is a custom drum.

*/

#include <bass.h>
#include <bassmidi.h>
#include <bassmix.h>

#include "BASSFX/Equalizer31BandFX.h"
#include "BASSFX/ReverbFX.h"
#include "BASSFX/ChorusFX.h"

#include "Midi/MidiHelper.h"

#include <QObject>
#include <QMap>


struct Instrument
{
    InstrumentType type;
    bool mute;
    bool solo;
    bool enable;
    int volume;
    int bus;
};


class MidiSynthesizer
{
public:
    MidiSynthesizer();
    ~MidiSynthesizer();

    bool isOpened() { return openned; }
    QStringList soundfontFiles() { return sfFiles; }

    bool open();
    void close();

    int outPutDevice();
    bool setOutputDevice(int dv);
    void setSoundFonts(QStringList &soundfonsFiles);
    void setVolume(float vol);
    float volume() { return synth_volume; }

    float soundfontVolume(int sfIndex);
    void setSoundfontVolume(int sfIndex, float sfvl);

    // std::vector<int> size 129
    //      1-128 all intrument
    //      129 is drum
    bool setMapSoundfontIndex(QList<int> intrumentSfIndex, QList<int> drumSfIndex);
    QList<int> getMapSoundfontIndex() { return instmSf; }
    QList<int> getDrumMapSfIndex() { return drumSf; }


    void sendNoteOff(int ch, int note, int velocity);
    void sendNoteOn(int ch, int note, int velocity);
    void sendNoteAftertouch(int ch, int note, int value);
    void sendController(int ch, int number, int value);
    void sendProgramChange(int ch, int number);
    void sendChannelAftertouch(int ch, int value);
    void sendPitchBend(int ch, int value);
    void sendAllNotesOff(int ch);
    void sendAllNotesOff();
    void sendResetAllControllers(int ch);
    void sendResetAllControllers();


    // Instrument Maper
    QMap<InstrumentType, Instrument> instrumentMap() { return instMap; }
    int volume(InstrumentType t);
    bool isMute(InstrumentType t);
    bool isSolo(InstrumentType t);
    void setVolume(InstrumentType t, int volume);
    void setMute(InstrumentType t, bool m);
    void setSolo(InstrumentType t, bool s);


    static std::vector<std::string> audioDevices();
    static bool isSoundFontFile(const QString &sfile);

    // Fx ----------------------
    Equalizer31BandFX* equalizer31BandFX() { return eq; }
    ReverbFX* reverbFX() { return reverb; }
    ChorusFX* chorusFX() { return chorus; }
    // ------------------------------------------


    bool isUseFloattingPoint() { return useFloat; }
    void setUsetFloattingPoint(bool use);

    bool isUseFXRC() { return useFX; }
    void setUseFXRC(bool use);


private:
    HSTREAM mixHandle;
    QMap<InstrumentType, HSTREAM> handles;
    QList<HSOUNDFONT> synth_HSOUNDFONT;
    QStringList sfFiles;
    QList<int> instmSf;
    QList<int> drumSf;
    QMap<InstrumentType, Instrument> instMap;
    InstrumentType chInstType[16];

    // FX
    Equalizer31BandFX *eq;
    ReverbFX *reverb;
    ChorusFX *chorus;
    // ---------------------------

    float synth_volume = 1.0f;
    bool openned = false;
    bool useSolo = false;

    int outDev = 1;
    bool useFloat = true;
    bool useFX = false;

    void setSfToStream();
    void calculateEnable();
    HSTREAM getDrumHandleFromNote(int drumNote);
    std::vector<int> getChannelsFromType(InstrumentType t);
};

#endif // MIDISYNTHESIZER_H
