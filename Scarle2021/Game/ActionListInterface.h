#pragma once

namespace AL
{
	struct UI
	{
		enum Action
		{
			unknown = 0, //Not handled

			//resize all UI
			resize_ui,
			
			//UI windows
			window_gate,
			window_karma_station,
			window_topias,
			window_training_centre,
			window_ports,
			window_siphon,
			window_special_buildings,
			window_omnibolges_love_domes,
			window_limbo_structures,
			window_delete_structures,
			window_advisors,
			window_soulview,

			//ui boarder UI
			window_file,
			window_global,
			window_window,
			window_tutorial,
			
			//file window
			file_Save,
			file_load,
			file_load_scenario,
			file_delete_game,
			file_new_game,
			file_exit_game,
			
			//zoom buttons
			button_zoom_in,
			button_zoom_out,
			
			//adviser buttons
			adv_option1,
			adv_option2,
			adv_option3,
			adv_option4,
			adv_option5
		};
	};
}