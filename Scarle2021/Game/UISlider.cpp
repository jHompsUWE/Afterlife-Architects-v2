#include "pch.h"
#include "UISlider.h"

UISlider::UISlider(int start_perc, float scale)
{
	curr_slider_percentage = start_perc;
	prev_slider_percentage = start_perc;
	slider_scale = scale * 1.84f;
}

float UISlider::updateSlider(const int perc_change, const float window_res_x)
{
	curr_slider_percentage += perc_change;
	if (curr_slider_percentage > 100)
	{
		curr_slider_percentage = 100;
	}
	else if (curr_slider_percentage < 0)
	{
		curr_slider_percentage = 0;
	}
	float position_change = (curr_slider_percentage - prev_slider_percentage) * slider_scale * window_res_x / base_window_res_x;
	prev_slider_percentage = curr_slider_percentage;
	return position_change;
}
