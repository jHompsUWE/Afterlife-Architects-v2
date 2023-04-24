#pragma once
#include "UIWindow.h"
#include "UISlider.h"

class UIWindowAudioSettings : public UIWindow
{
public:
    UIWindowAudioSettings(Vector2 _windowPosition, ID3D11Device* _d3dDevice,
        std::string _text, std::string _filepath, Vector2 _setScale);

    ~UIWindowAudioSettings() override;

    void update(GameData* _gameData, Vector2& _mousePosition) override;
    void render(DrawData2D* _drawData) override;
    bool ReceiveEvents(const AL::Event& al_event) override;

    void reSize(Vector2 game_res) override;

private:
    void updateSlider(int slider_index, int perc_change);
    std::vector<ImageGO2D*> image_vec;
    std::vector<UISlider*> sliders; //0 master, 1 music, 2 sound
    AL::UI::Action actions[6] = { AL::UI::audio_master_left, AL::UI::audio_master_right, AL::UI::audio_music_left, AL::UI::audio_music_right, AL::UI::audio_sound_left, AL::UI::audio_sound_right };
};

