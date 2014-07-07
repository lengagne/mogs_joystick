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

#include <string.h>
#include <stdlib.h>
#include <iostream>

MoGS_Joystick::MoGS_Joystick ()
{
	MoGS_Config_Joystick config_finder_;
	if ( ! config_finder_.read_config())
	{
		std::cerr <<"MoGS_Joystick is not configured, please run the \"MoGS_Config_Joystick\" executable (certainly as root)"<< std::endl;
		config_finder_.create_config();
	}
	bool undefined_joystick = true;
	bool next_joystick = true;
	int cpt = 0;
	Joystick_ = NULL;
	while ( undefined_joystick && next_joystick)
	{
		char tmp_path[20];
		sprintf(tmp_path,"/dev/input/js%d",cpt);
		if (Joystick_)
		{
			delete Joystick_;
		}
		Joystick_ = new cJoystick ( );
		if (Joystick_->init(tmp_path))
		{		
			if ( config_finder_.has_pad(std::string(Joystick_->name)))
			{
				config_ = config_finder_.get_pad_config(Joystick_->name);
				undefined_joystick = false;
			}
		}else{
			next_joystick = false;
		}
		cpt++;
	}	
	if (undefined_joystick)
	{
		std::cerr <<"There is no known pad, please run the \"MoGS_Config_Joystick\" executable (certainly as root)"<< std::endl;
	}
	
	// init the stop and pause button
	Joystick_->joystick_st->button[config_.stop_button.id] = false;
	Joystick_->joystick_st->button[config_.pause_button.id] = false;
}

MoGS_Joystick::~MoGS_Joystick ()
{
	delete Joystick_;
}

double MoGS_Joystick::get_double( const type & in) const
{
	if (in.push == AXIS)
	{
		return (in.sign * (((double) Joystick_->joystick_st->axis[in.id]) / (SHRT_MAX)));
	}else
	{
		return Joystick_->joystick_st->button[in.id] - Joystick_->joystick_st->button[in.id_neg];
	}
}

/// use to stop the process
bool MoGS_Joystick::get_stop ()
{
	return (Joystick_->joystick_st->button[config_.stop_button.id]);
}

/// use to pause the process
bool
MoGS_Joystick::get_pause ()
{
	return (Joystick_->joystick_st->button[config_.pause_button.id]);
}

/// get the forward velocity (positive if forward, negative if backward)
double MoGS_Joystick::get_forward_velocity ()
{
	return get_double(config_.forward);
}

/// get the side velocity (positive if one the right, negative if on the left)
double
MoGS_Joystick::get_side_velocity ()
{
 	return get_double(config_.side);
}

/// get the rotate velocity (positive if one the right, negative if on the left)
double
MoGS_Joystick::get_rotate_velocity ()
{
	return get_double(config_.rotate);
}

/// get the up velocity (positive if up, negative if down)
double
MoGS_Joystick::get_up_velocity ()
{
	return get_double(config_.up);
}
