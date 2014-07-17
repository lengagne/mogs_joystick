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

#include <boost/filesystem.hpp>
#include <unistd.h>
#include <fstream>
#include <string>

// Constants:
const std::string CONFIG_FILENAME( "MoGS_Config_Joystick.xml" ) ;

// namespace for clarity :
namespace fs = boost::filesystem ;

/*!
 * Constructor
 */
MoGS_Config_Joystick::MoGS_Config_Joystick()
{
    fs::path home( getenv("HOME") ) ;
    fs::path locpath( home / ".config/MoGs" ) ;
    
    search_paths.push_back( locpath ) ;
    search_paths.push_back( fs::path( (std::string)CONFIG_REPOSITORY ) );
}

/*!
 * Destructor
 */
MoGS_Config_Joystick::~MoGS_Config_Joystick ()
{
    
}

/*!
 * 
 */
void MoGS_Config_Joystick::add_button( const std::string & balise , const type & in , tinyxml2::XMLElement * El )
{
    tinyxml2::XMLElement * ElAdd = doc.NewElement (balise.c_str());
    tinyxml2::XMLElement * ElId = doc.NewElement ("id");
    tinyxml2::XMLText *ElText = doc.NewText ((std::to_string(in.id)).c_str());
    ElId->InsertEndChild (ElText);
    ElAdd->InsertEndChild (ElId);
    El->InsertEndChild (ElAdd);
}

/*!
 * 
 */
void MoGS_Config_Joystick::add_axis_button( const std::string & balise , const type & in , tinyxml2::XMLElement * El )
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
    } else
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

/*!
 * 
 */
void MoGS_Config_Joystick::add_Joystick(const pad_control & info)
{
    std::string filename = get_save_filename() ;
    
    int loadOkay = doc.LoadFile (filename.c_str ());
    if (loadOkay != tinyxml2::XML_NO_ERROR)
    {
        std::cerr << "Error in " << __FILE__ << " line:" << __LINE__ << " when opening " << filename << std::endl;
        std::cerr << "doc_.LoadFile returns :" << loadOkay << std::endl;
        exit (0);
    }
    tinyxml2::XMLElement * root = doc.RootElement ();
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
    int saveOkay = doc.SaveFile (filename.c_str ());
    if ( saveOkay != tinyxml2::XML_NO_ERROR )
    {
        std::cerr << "Error in " << __FILE__ << " line:" << __LINE__ << " when saving " << filename << std::endl;
        std::cerr << "doc_.SaveFile returns :" << saveOkay << std::endl;
        exit (0);
    }
}

/*!
 * 
 */
void MoGS_Config_Joystick::create_config() const
{   
    std::string filename = get_save_filename() ;
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    tinyxml2::XMLElement * root = doc.NewElement ("Joysticks");
    doc.LinkEndChild( root );
    doc.SaveFile (filename.c_str());
}

/*!
 * 
 */
pad_control MoGS_Config_Joystick::get_pad_config(const std::string name) const
{
    for( pad_control pad : pads_ )
    {
        if ( pad.name == name ) 
            return pad ;
    }
    
    std::cerr<<"Error in "<<__FILE__<<" at line "<<__LINE__<<"."<<std::endl;
    std::cerr<<"We cannot find the pad: "<< name <<std::endl;
    exit(0);
}

/*!
 * 
 */
bool MoGS_Config_Joystick::has_pad(const std::string name) const
{
    for( pad_control pad : pads_ )
    {
        if ( pad.name == name ) 
            return true ;
    }
    return false;
}

/*!
 * 
 */
type MoGS_Config_Joystick::get_push( const std::string & question, cJoystick * Joystick) const
{
    char c;
    int id = -1, id_max;
    type out;
    do {
        std::cout<<question;
        std::cout<<"\tA= Axis\tB= Button"<<std::endl;
        int dummy = scanf("%c",&c);
        if ( c == 'A')
        {
            out.push = AXIS;
            id_max = Joystick->axes;
        } else if (c == 'B')
        {
            out.push = BUTTON;
            id_max = Joystick->buttons;
        }
    } while (c!='A' && c!='B');

    do {
        if (out.push == AXIS)
        {
            std::cout<<question;
            std::cout<<"\tid ? "<<std::endl;
            int dummy = scanf("%d",&id);
            out.id = id;
            out.id_neg = id;

            do {
                std::cout<<question;
                std::cout<<"\tsign (+ or -)"<<std::endl;
                int dummy = scanf("%c",&c);
                if ( c == '+')
                {
                    out.sign = 1.0;
                } else if (c == '-')
                {
                    out.sign = -1;
                }
            } while (c!='+' && c!='-');

        } else
        {
            std::cout<<question;
            std::cout<<"\tPlease give the id for positive value"<<std::endl;
            int dummy = scanf("%d",&id);
            out.id = id;
            std::cout<<"\tPlease give the id for negative value"<<std::endl;
            dummy = scanf("%d",&id);
            out.id_neg = id;

        }
    } while ( out.id < 0 || out.id > id_max || out.id_neg < 0 || out.id_neg > id_max);
    std::cout<<"*************************"<<std::endl;
    std::cout<<"*************************"<<std::endl;
    return out;
}

/*!
 * 
 */
type MoGS_Config_Joystick::get_axis_button( const std::string & balise , tinyxml2::XMLElement * El )
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

    } 
    else if (attribute == "button")
    {
        tmp.push = BUTTON;
        tinyxml2::XMLElement * ElId = ElData->FirstChildElement ("id");
        tmp.id = string_to_int(ElId->GetText());
        tinyxml2::XMLElement * ElId_neg = ElData->FirstChildElement ("id_neg");
        tmp.id_neg = string_to_int(ElId_neg->GetText());

    } 
    else
    {
        std::cerr<<"Error in "<<__FILE__<<" at line "<<__LINE__<<std::endl;
        std::cerr<<"the attribute "<<  attribute <<" in "<< balise<<" must ne axis or button."<<std::endl;
        exit(0);
    }
    return tmp;
}

/*!
 * 
 */
type MoGS_Config_Joystick::get_button( const std::string & balise , tinyxml2::XMLElement * El )
{
    type tmp;
    tmp.push = BUTTON;
    tinyxml2::XMLElement * ElData = El->FirstChildElement (balise.c_str());
    tinyxml2::XMLElement * ElId = ElData->FirstChildElement ("id");
    tmp.id = string_to_int(ElId->GetText());
    return tmp;
}

/*!
 * 
 */
type MoGS_Config_Joystick::get_push_button( const std::string & question, cJoystick * Joystick) const
{
    char c;
    int id = -1, id_max;
    id_max = (int) Joystick->buttons;
    type out;
    out.push = BUTTON;
    do {
        std::cout<<question;
        std::cout<<"\t Please give the id "<<std::endl;
        int dummy = scanf("%d",&id);
        out.id = id;
    } while ( out.id < 0 || out.id > id_max );
    std::cout<<"*************************"<<std::endl;
    std::cout<<"*************************"<<std::endl;
    return out;
}

/*!
 * 
 */
bool MoGS_Config_Joystick::read_config()
{
    if ( !config_file_exists() )
        return false ;

    std::string filename = get_open_filename() ;
    std::cout << "Reading config from " << filename << std::endl ;
    
    std::string cmd = "xmllint --dtdvalid " + (std::string) DTD_REPOSITORY + "/mogs_joystick.dtd --noout " + filename;
    int retCode = system (cmd.c_str ());
    if (retCode != 0)
    {
        std::cerr << "Error when execute " << cmd << "  (to check the XML file)" << std::endl;
        return false;
    }

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

/*!
 * Check if config file exists in the search paths
 */
bool MoGS_Config_Joystick::config_file_exists() const 
{
    for( fs::path p : search_paths ) 
    {
        if ( fs::exists( p / CONFIG_FILENAME ) )
            return true ;
    }
    return false ;
}

/*!
 * \brief get filename for opening 
 * 
 * If user is root file is taken from CONFIG_REPOSITORY
 * else we go thru the search paths
 */
std::string MoGS_Config_Joystick::get_open_filename() const
{
    if (!getuid()) // user is root
    {
        return (std::string)CONFIG_REPOSITORY + CONFIG_FILENAME ;
    }
    else
    {
        for( fs::path p : search_paths )
        {
            fs::path fp( p / CONFIG_FILENAME ) ;
            if ( fs::exists( fp ) )
                return fp.string() ;
        }
    }
    
    return std::string() ;
}

/*!
 * \brief get filename for saving
 * 
 * Behaviour depends on the user :
 * user is root -> file is saved in CONFIG_REPOSITORY 
 * user is NOT root -> file is saved in home directory (~/.config/MoGS)
 * 
 * If local path doesn't exists, it is created first. :)
 */
std::string MoGS_Config_Joystick::get_save_filename() const
{
    if (!getuid()) // user is root
    {
        return (std::string)CONFIG_REPOSITORY + CONFIG_FILENAME ;
    }
    else
    {
        fs::path homepath( getenv("HOME") ) ;
        fs::path locpath( homepath / ".config/MoGs" ) ;
        if ( !fs::exists( locpath ) )
        {
            fs::create_directory( locpath );
            search_paths.insert( search_paths.begin() , locpath );
        }
        return (locpath/CONFIG_FILENAME).string() ;
    }
}