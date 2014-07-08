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
#include <unistd.h>
#include <fstream>
#include <string>



MoGS_Config_Joystick::MoGS_Config_Joystick ()
{

}

MoGS_Config_Joystick::~MoGS_Config_Joystick ()
{
	
}

void MoGS_Config_Joystick::add_button(std::string balise,
		type in,
		tinyxml2::XMLElement * El)
{
	tinyxml2::XMLElement * ElAdd = doc.NewElement (balise.c_str());
	tinyxml2::XMLElement * ElId = doc.NewElement ("id");
	tinyxml2::XMLText *ElText = doc.NewText ((std::to_string(in.id)).c_str());
	ElId->InsertEndChild (ElText);
	ElAdd->InsertEndChild (ElId);
	El->InsertEndChild (ElAdd);
}

void MoGS_Config_Joystick::add_axis_button(std::string balise,
		type in, 				     
		tinyxml2::XMLElement * El)
{
	tinyxml2::XMLElement * ElAdd = doc.NewElement (balise.c_str());
	if (in.push == AXIS)
	{
		ElAdd->SetAttribute("type","axis");
		tinyxml2::XMLElement * ElId = doc.NewElement ("id");
		tinyxml2::XMLText *ElText = doc.NewText ((std::to_string(in.id)).c_str());
		ElId->InsertEndChild (ElText);
		ElAdd->InsertEndChild (ElId);
		
		tinyxml2::XMLElement * ElSign = doc.NewElement ("sign");
		ElText = doc.NewText ((std::to_string(in.sign)).c_str());
		ElSign->InsertEndChild (ElText);
		ElAdd->InsertEndChild (ElSign);
	}else
	{
		ElAdd->SetAttribute("type","button");
		tinyxml2::XMLElement * ElId = doc.NewElement ("id");
		tinyxml2::XMLText *ElText = doc.NewText ((std::to_string(in.id)).c_str());
		ElId->InsertEndChild (ElText);
		ElAdd->InsertEndChild (ElId);
		
		tinyxml2::XMLElement * ElId_neg = doc.NewElement ("id_neg");
		ElText = doc.NewText ((std::to_string(in.id_neg)).c_str());
		ElId_neg->InsertEndChild (ElText);
		ElAdd->InsertEndChild (ElId_neg);
	}
	El->InsertEndChild (ElAdd);
}


void MoGS_Config_Joystick::add_Joystick(const pad_control & info)
{
	// FIXME : check if the user is root (sometimes does not needed)
	int user = getuid();
	if (user != 0)
	{
		std::cerr<<"Error you must be root to add a joystick to the configuration file !! "<<std::endl;
		exit(0);
	}
	
	std::string filename = (std::string) CONFIG_REPOSITORY + "/MoGS_Config_Joystick.xml";
	
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
	
	
	add_button("stop_button",info.stop_button,ElJoystick);
	add_button("pause_button",info.pause_button,ElJoystick);
	add_axis_button("forward",info.forward,ElJoystick);
	add_axis_button("side",info.side,ElJoystick);
	add_axis_button("up",info.up,ElJoystick);
	add_axis_button("rotate",info.rotate,ElJoystick);
	
	
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
		std::cout<<question;
		std::cout<<"\tA= Axis\tB= Button"<<std::endl;
		int dummy = scanf("%c",&c);
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
		if (out.push == AXIS)
		{
			std::cout<<question;
			std::cout<<"\tid ? "<<std::endl;
			int dummy = scanf("%d",&id);
			out.id = id;
			out.id_neg = id;
			
			do{
				std::cout<<question;
				std::cout<<"\tsign (+ or -)"<<std::endl;
				int dummy = scanf("%c",&c);
				if ( c == '+')
				{
					out.sign = 1.0;
				}else if (c == '-')
				{
					out.sign = -1;
				}
			}while (c!='+' && c!='-');
			
		}else
		{
			std::cout<<question;
			std::cout<<"\tPlease give the id for positive value"<<std::endl;
			int dummy = scanf("%d",&id);
			out.id = id;
			std::cout<<"\tPlease give the id for negative value"<<std::endl;
			dummy = scanf("%d",&id);
			out.id_neg = id;
			
		}
	}while ( out.id < 0 || out.id > id_max || out.id_neg < 0 || out.id_neg > id_max);
	std::cout<<"*************************"<<std::endl;
	std::cout<<"*************************"<<std::endl;
	return out;
}

type MoGS_Config_Joystick::get_axis_button(	std::string balise,
						tinyxml2::XMLElement * El)
{
	type tmp;

	tinyxml2::XMLElement * ElData = El->FirstChildElement (balise.c_str());
	
	std::string attribute = char_to_string(ElData->Attribute("type"));
	if (attribute == "axis")
	{
		tmp.push = AXIS;
		tinyxml2::XMLElement * ElId = ElData->FirstChildElement ("id");
		tmp.id = string_to_int(ElId->GetText());
		tinyxml2::XMLElement * Elsign = ElData->FirstChildElement ("sign");
		tmp.sign = string_to_double(Elsign->GetText());

	}else if (attribute == "button")
	{
		tmp.push = BUTTON;
		tinyxml2::XMLElement * ElId = ElData->FirstChildElement ("id");
		tmp.id = string_to_int(ElId->GetText());
		tinyxml2::XMLElement * ElId_neg = ElData->FirstChildElement ("id_neg");
		tmp.id_neg = string_to_int(ElId_neg->GetText());

	}else
	{
		std::cerr<<"Error in "<<__FILE__<<" at line "<<__LINE__<<std::endl;
		std::cerr<<"the attribute "<<  attribute <<" in "<< balise<<" must ne axis or button."<<std::endl;
		exit(0);
	}
	return tmp;	
}
	
type MoGS_Config_Joystick::get_button(	std::string balise,
					tinyxml2::XMLElement * El)
{
	type tmp;
	tmp.push = BUTTON;
	tinyxml2::XMLElement * ElData = El->FirstChildElement (balise.c_str());
	tinyxml2::XMLElement * ElId = ElData->FirstChildElement ("id");
	tmp.id = string_to_int(ElId->GetText());
	return tmp;
}

type MoGS_Config_Joystick::get_push_button(const std::string question, cJoystick * Joystick) const
{
	char c;
	int id = -1, id_max;
	id_max = (int) Joystick->buttons;
	type out;
	out.push = BUTTON;
	do{
		std::cout<<question;
		std::cout<<"\t Please give the id "<<std::endl;
		int dummy = scanf("%d",&id);
		out.id = id;
	}while ( out.id < 0 || out.id > id_max );
	std::cout<<"*************************"<<std::endl;
	std::cout<<"*************************"<<std::endl;
	return out;
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
	
	tinyxml2::XMLDocument doc;
	bool loadOkay = doc.LoadFile (filename.c_str ());
	if (loadOkay != tinyxml2::XML_NO_ERROR)
	{
		std::cerr << "Error in " << __FILE__ << " line:" << __LINE__ << std::endl;
		std::cerr << "doc.LoadFile returns :" << loadOkay << std::endl;
		exit (0);
	}
	
	tinyxml2::XMLElement * root = doc.RootElement ();
	int cpt = 0;
	tinyxml2::XMLElement * ElChild = root->FirstChildElement ("Joystick");
	for (ElChild; ElChild; ElChild = ElChild->NextSiblingElement ("Joystick"))
	{
		pad_control tmp;
		cpt ++;
		tinyxml2::XMLElement * ElName = ElChild->FirstChildElement ("name");
		tmp.name = char_to_string(ElName->GetText ());
		tmp.stop_button = get_button("stop_button",ElChild);
		tmp.pause_button = get_button("pause_button",ElChild);
		tmp.forward = get_axis_button("forward",ElChild);
		tmp.side = get_axis_button("side",ElChild);
		tmp.up = get_axis_button("up",ElChild);
		tmp.rotate = get_axis_button("rotate",ElChild);
		pads_.push_back(tmp);
	}
	
	return true;
}

