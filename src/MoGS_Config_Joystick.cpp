//      MoGS_Config_Joystick.cpp
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

#include "config_Joystick.h"
#include <fstream>

MoGS_Config_Joystick::MoGS_Config_Joystick ()
{

}

MoGS_Config_Joystick::~MoGS_Config_Joystick ()
{
	
}

void MoGS_Config_Joystick::add_Joystick(const pad_control & info) const
{
	// FIXME : check if the user is root (sometimes does not needed)
	int user = getuid();
	if (user != 0)
	{
		std::cerr<<"Error you must be root to add a joystick to the configuration file !! "<<std::endl;
		exit(0);
	}
	
	std::string filename = (std::string) CONFIG_REPOSITORY + "/MoGS_Config_Joystick.xml";
	std::cout<<" We are reading the file : "<< filename<<std::endl;
	tinyxml2::XMLDocument doc;
	int loadOkay = doc.LoadFile (filename.c_str ());
	if (loadOkay != tinyxml2::XML_NO_ERROR)
	{
		std::cerr << "Error in " << __FILE__ << " line:" << __LINE__ << " when opening " << filename << std::endl;
		std::cerr << "doc_.LoadFile returns :" << loadOkay << std::endl;
		exit (0);
	}	
	tinyxml2::XMLElement *root = doc.RootElement ();		
	
	
	
	tinyxml2::XMLElement * ElJoystick = doc.NewElement ("Joystick");
	tinyxml2::XMLElement * ElName = doc.NewElement ("name");
	tinyxml2::XMLText *ElText = doc.NewText ( info.name.c_str());
	ElName->InsertEndChild (ElText);
	ElJoystick->InsertEndChild (ElName);	
	root->InsertEndChild ( ElJoystick ); 
	doc.SaveFile (filename.c_str ());
}

void MoGS_Config_Joystick::create_config() const
{
	// FIXME : check if the user is root (sometimes does not needed)
	int user = getuid();
	if (user != 0)
	{
		std::cerr<<"Error you must be root to create the configuration file !! "<<std::endl;
		exit(0);
	}
	
	std::string filename = (std::string) CONFIG_REPOSITORY + "/MoGS_Config_Joystick.xml";
	std::cout<<" We are creating the file : "<< filename<<std::endl;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");  
	tinyxml2::XMLElement * root = doc.NewElement ("Joysticks");
	doc.LinkEndChild( root ); 	
	doc.SaveFile (filename.c_str ());
}
pad_control MoGS_Config_Joystick::get_pad_config(const std::string name) const
{
	int nb = pads_.size();
	for (int i=0;i<nb;i++)
	{
		if (pads_[i].name == name)
			return pads_[i];
	}
	std::cerr<<"Error in "<<__FILE__<<" at line "<<__LINE__<<"."<<std::endl;
	std::cerr<<"We cannot find the pad: "<< name <<std::endl;
	exit(0);
}


bool MoGS_Config_Joystick::has_pad(const std::string name) const
{
	int nb = pads_.size();
	for (int i=0;i<nb;i++)
	{
		if (pads_[i].name == name)
			return true;
	}
	return false;
}

type MoGS_Config_Joystick::get_push(const std::string question, cJoystick * Joystick) const
{
	char c;
	int id = -1, id_max;
	type out;
	do{
		std::cout<<question<<std::endl;
		std::cout<<"\tA= Axis\tB= Button"<<std::endl;
		scanf("%c",&c);
		if ( c == 'A')
		{
			out.push = AXIS;
			id_max = Joystick->axes;
		}else if (c == 'B')
		{
			out.push = BUTTON;
			id_max = Joystick->buttons;
		}
	}while (c!='A' && c!='B');
	
	do{
		std::cout<<question<<std::endl;
		std::cout<<"\tid ? "<<std::endl;
		scanf("%d",&id);
		out.id = id;
	}while ( id < 0 || id > id_max);
	do{
		std::cout<<question<<std::endl;
		std::cout<<"\tsign (+ or -)"<<std::endl;
		scanf("%c",&c);
		if ( c == '+')
		{
			out.sign = 1.0;
		}else if (c == '-')
		{
			out.sign = -1;
		}
	}while (c!='+' && c!='-');
	return out;
	
	// FIXME en cas de boutton, il y a deux valeurs
	
}

bool MoGS_Config_Joystick::read_config()
{
	std::string filename = (std::string) CONFIG_REPOSITORY + "/MoGS_Config_Joystick.xml";
	std::ifstream fichier;
	fichier.open (filename);
	if (fichier == 0)
	{
		return false;
	}
	fichier.close ();	
	
	
	std::string cmd = "xmllint --dtdvalid " + (std::string) DTD_REPOSITORY + "/mogs_joystick.dtd --noout " + filename;
	int retCode = system (cmd.c_str ());
	if (retCode != 0)
	{
		std::cerr << "Error when execute " << cmd << "  (to check the XML file)" << std::endl;
// 		exit (0);
		return false;
	}	
	
	return true;
}

