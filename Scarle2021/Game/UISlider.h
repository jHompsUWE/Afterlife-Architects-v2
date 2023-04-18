#pragma once
#include "ImageGO2D.h"

class UISlider
{
public:
    UISlider(int start_perc, float scale);
    float updateSlider(const int perc_change, const float window_res_x);
    int getPercent() { return curr_slider_percentage; };
private:
    int base_window_res_x = 352;
    int curr_slider_percentage;
    int prev_slider_percentage;
    float slider_scale;
};

