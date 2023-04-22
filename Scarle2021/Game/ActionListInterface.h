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
            window_macro_manager,
			window_graphview,

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

			//global window
			global_audio_settings_window,
			global_tempo_window,
			global_bad_things_window,
			global_misc_options_window,

			//bad things window
			no_bad_things,
			birds_of_paradise,
			bats_out_of_hell,
			my_blue_heaven,
			hell_freezes_over,
			heaven_nose,
			hell_in_a_hand_basket,
			paradise_pair_of_dice,
			disco_inferno,

			//bad thing trigger
			event_activated,
			event_canceled,
			
			//audio settings
			audio_master_left,
			audio_master_right,
			audio_sound_left,
			audio_sound_right,
			audio_music_left,
			audio_music_right,
			
			//zoom buttons
			button_zoom_in,
			button_zoom_out,
			
			//adviser buttons
			adv_option1,
			adv_option2,
			adv_option3,
			adv_option4,
			adv_option5,

			//micro manager
			micro_manager_arrow_l,
			micro_manager_arrow_r,

			//graphview
			graphview_bel,
			graphview_rel,
			graphview_pop
		};
	};
}