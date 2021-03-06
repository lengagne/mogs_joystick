//      Config_PAD.cpp
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

#include "MoGS_Config_Joystick.h"

int main ()
{
	std::cout<<"Configuration of the pad"<<std::endl;
	MoGS_Config_Joystick config_finder_;
	pad_control config;
	
	if ( ! config_finder_.read_config())
	{
		config_finder_.create_config();
	}
	
	bool undefined_joystick = true;
	bool next_joystick = true;
	int cpt = 0;
	cJoystick * Joystick = NULL;
	char tmp_path[20];
	while ( undefined_joystick && next_joystick)
	{
		sprintf(tmp_path,"/dev/input/js%d",cpt);
		if (Joystick)
			delete Joystick;
		Joystick = new cJoystick ( );
		if (Joystick->init(tmp_path))
		{		
			std::string name = Joystick->name;
			// check compatibility
			if ((int) Joystick->axes >= 2 && (int) Joystick->buttons >= 6 && !config_finder_.has_pad(name))
			{
				undefined_joystick = false;
			}
		}else{
			next_joystick = false;
		}
		cpt++;
	}	
	if (undefined_joystick)
	{
		std::cerr <<"There is no unconfigured pad"<< std::endl;
	}else
	{
		int nb_axis = (int) Joystick->axes;
		int nb_bouton = (int) Joystick->buttons;
		
		std::cout<<"We will add the pad : "<<Joystick->name <<std::endl;
		std::cout<<"The pad has "<<nb_axis <<" axis and "<< nb_bouton <<" boutons."<<std::endl;
		std::cout<<"You can use the \"./test_pad "<< tmp_path <<"\" to know which button is needed "<<std::endl;
		
		config.name = Joystick->name;
		config.stop_button = config_finder_.get_push_button("\t Configuration of the stop button",Joystick);
		config.pause_button = config_finder_.get_push_button("\t Configuration of the pause button",Joystick);
		config.forward = config_finder_.get_push("\t Configuration of the forward buttons/axis",Joystick);
		config.side = config_finder_.get_push("\t Configuration of the side buttons/axis",Joystick);
		config.up = config_finder_.get_push("\t Configuration of the up buttons/axis",Joystick);
		config.rotate = config_finder_.get_push("\t Configuration of the rotate buttons/axis",Joystick);
		
		config_finder_.add_Joystick(config);
                config_finder_.save_config() ;
	}
	
	return 1;
}
