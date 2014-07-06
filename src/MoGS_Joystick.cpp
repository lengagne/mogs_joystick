//      MoGS_Joystick.cpp
//      Copyright (C) 2012 lengagne (lengagne@gmail.com)
// 
//      This program is free software: you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation, either version 3 of the License, or
//      (at your option) any later version.
// 
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
// 
//      You should have received a copy of the GNU General Public License
//      along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//      This program was developped in the following labs:
//      from 2012: IUT de Beziers/ LIRMM, Beziers, France

#include "MoGS_Joystick.h"
#include "MoGS_Config_Joystick.h"

#include "GreenAsian_Sanwa_pad.h"
#include "Mega_World_USB_2_Axis_8_Button_Gamepad.h"
#include "XBox_pad.h"
#include "HORI_PAD_3_TURBO.h"

#include <string.h>
#include <stdlib.h>
#include <iostream>

MoGS_Joystick::MoGS_Joystick ()
{
	MoGS_Config_Joystick config_finder_;
	if ( ! config_finder_.read_config())
	{
		config_finder_.create_config();
	}
	bool undefined_joystick = true;
	bool next_joystick = true;
	int cpt = 0;
	cJoystick * Joystick = NULL;
	while ( undefined_joystick && next_joystick)
	{
		std::cout<<"cpt = "<<cpt<<std::endl;
		char tmp_path[20];
		sprintf(tmp_path,"/dev/input/js%d",cpt);
		if (Joystick)
			delete Joystick;
		Joystick = new cJoystick ( );
		if (Joystick->init(tmp_path))
		{		
			std::string name = Joystick->name;
			if ( config_finder_.has_pad(name))
			{
				config_ = config_finder_.get_pad_config(name);
				undefined_joystick = false;
			}
		}else{
			next_joystick = false;
			std::cout<<"next_joystick = "<< next_joystick <<std::endl;
		}
		cpt++;
	}	
	if (undefined_joystick)
	{
		std::cerr <<"There is no known pad, please run the \"MoGS_Config_Joystick\" executable (certainly as root)"<< std::endl;
	}
// 	Joystick->check();
}

MoGS_Joystick::~MoGS_Joystick ()
{
	delete Joystick;
}

/// use to stop the process
bool
MoGS_Joystick::get_stop ()
{
// 	return (Joystick->get_stop ());
}

/// use to pause the process
bool
MoGS_Joystick::get_pause ()
{
// 	return (Joystick->get_pause ());
}

/// get the forward velocity (positive if forward, negative if backward)
double
MoGS_Joystick::get_forward_velocity ()
{
// 	return (Joystick->get_forward_velocity ());
}

/// get the side velocity (positive if one the right, negative if on the left)
double
MoGS_Joystick::get_side_velocity ()
{
// 	return (Joystick->get_side_velocity ());
}

/// get the rotate velocity (positive if one the right, negative if on the left)
double
MoGS_Joystick::get_rotate_velocity ()
{
// 	return Joystick->get_rotate_velocity ();
}

/// get the up velocity (positive if up, negative if down)
double
MoGS_Joystick::get_up_velocity ()
{
// 	return Joystick->get_up_velocity ();
}
