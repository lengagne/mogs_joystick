//      MoGS_Config_Joystick.h
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
//	from 2013 : Université Blaise Pascal / axis : ISPR / theme MACCS

#ifndef __MOGS_CONFIG_JOYSTICK_H__
#define __MOGS_CONFIG_JOYSTICK_H__

#include <string>
#include <vector>

#include "MoGS_Joystick.h"
#include "additionnal_tinyxml.h"


class MoGS_Config_Joystick
{
      public:
	MoGS_Config_Joystick ();

	~MoGS_Config_Joystick ();
	
	void add_Joystick(const pad_control & info) const;

	void create_config() const;
	
	pad_control get_pad_config(const std::string name) const;
	
	bool has_pad(const std::string name) const;
	
	type get_push(const std::string question, cJoystick * Joystick) const;
	
	bool read_config();
	
      protected:
	      
      private:

	std::vector<pad_control> pads_;
};

#endif
