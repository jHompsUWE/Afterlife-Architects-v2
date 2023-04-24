
#include "pch.h" 
#include "AudioManager.h" 

#include <chrono>
#include <iostream>

AudioManager::AudioManager()
{
    //Subscribes to the event system! 
    AL::NewEventManager::AddEventReceiver(false, this, AL::EventType::event_sound_start, AL::EventType::event_sound_stop, AL::EventType::event_sound_volume);
}

AudioManager::~AudioManager()
{
    AL::NewEventManager::RemoveEventReceiver(this);
}

/// <summary>
/// Generating Audio Engine within initialisation
/// </summary>
/// <returns></returns>
bool AudioManager::init()
{
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
    audEngine = std::make_unique<AudioEngine>(eflags);
    ShuffleMusic();
    PlayMusic(music_index_array[current_music_index]);
    return true;
}

/// <summary>
/// Update the manager each frame to check if audio sources need to be deleted or created
/// </summary>
/// <param name="game_data"></param>
void AudioManager::Update(GameData* game_data)
{
    if (!audEngine->Update())
    {
        //more 
    }
    // If any sounds stop playing
    for (auto& s : sounds_list)
    {
        if (!s->GetPlayState())
        {
            /// Remove sound at this point
            temp_sounds.push_back(s);
        }
        s->SetVolume(master_volume * sound_volume);
    }
    for (auto& t : temp_sounds)
    {
        delete t;
        sounds_list.remove(t);
    }
    temp_sounds.clear();

    music->SetVolume(master_volume * music_volume);
    // If music stops
    if (!music->GetPlayState())
    {
        // If reaches last theme
        if (current_music_index == 6)
        {
            // Shuffle themes around and play again
            current_music_index = 0;
            ShuffleMusic();
        }
        else
        {
            // Otherwise go onto next theme
            current_music_index += 1;
        }
        PlayMusic(music_index_array[current_music_index]);
    }
}

/// <summary>
/// Get sound event from Event Manager
/// </summary>
/// <param name="al_event"></param>
const bool& AudioManager::ReceiveEvents(const AL::Event& al_event)
{
    switch (al_event.type)
    {
    case AL::event_sound_start:
        PlaySound(al_event.sound_start.filename);
        break;

    case AL::event_sound_volume:
        switch (al_event.sound_volume.volume_index)
        {
        case 0:
            master_volume = al_event.sound_volume.new_volume * 0.01f;
            break;
            
        case 1:
            music_volume = al_event.sound_volume.new_volume * 0.01f;
            break;
            
        case 2:
            sound_volume = al_event.sound_volume.new_volume * 0.01f;
            break;
        }
        break;
        
    default:
        break;
    }
    return false;
}


/// <summary>
/// Generate sound with given filename
/// </summary>
/// <param name="filename"></param>
void AudioManager::PlaySound(string filename)
{
    Sound* sound_eff = new Sound(audEngine.get(), filename, master_volume * sound_volume);
    sounds_list.push_back(sound_eff);
}

/// <summary>
/// Generate music with given index
/// </summary>
/// <param name="filename"></param>
void AudioManager::PlayMusic(int index)
{
    string temp_str = "Afterlife Theme " + std::to_string(index);
    Sound* sound_eff = new Sound(audEngine.get(), temp_str, master_volume * music_volume);
    music = sound_eff;
}

/// <summary>
/// Shuffle music index array around to produce different order played
/// </summary>
void AudioManager::ShuffleMusic()
{
    int temp_index = music_index_array[6];
    do {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(music_index_array.begin(), music_index_array.end(), std::default_random_engine(seed));
    } while (music_index_array[0] == temp_index);
}