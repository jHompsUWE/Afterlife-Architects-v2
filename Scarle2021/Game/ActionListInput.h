#pragma once

namespace AL
{
	struct Input
	{
		enum Action
		{
			unknown = 0, //Keyboard not handled

			//Build sys
			build_houses,
			show_vibes,
			place_zone_green,
			place_zone_yellow,
			place_zone_orange,
			place_zone_brown,
			place_zone_purple,
			place_zone_red,
			place_zone_blue,
			delete_zone,
			

			//Camera
			camera_up,
			camera_down,
			camera_left,
			camera_right
		};
	};
}