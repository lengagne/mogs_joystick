/*
 * Copyright (C) 2014  Clément Fouque <clement.fouque@univ-bpclermont.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <memory>

#include <QApplication>
// #include <QTranslator>
#include <QDebug>
#include <QMessageBox>

#include "src/JoystickMapper.h"
#include "Joystick.h"

int main (int argc, char *argv[])
{
    QApplication app( argc , argv ) ;
    
    // First, we lists the avaliable joypads :
    QMap<int,cJoystick*> avaliableJoysticks ;
    bool undefined_joystick = true;
    bool next_joystick = true ;
    int cpt = 0;
    char tmp_path[20];
    
    while ( next_joystick )
    {
        sprintf(tmp_path,"/dev/input/js%d",cpt) ;
        std::unique_ptr<cJoystick> js( new cJoystick ) ;
        
        if ( js->init(tmp_path) )
        {
            std::string name = js->name;
            // check compatibility
            if ((int) js->axes >= 2 && (int) js->buttons >= 6 )
            {
                qDebug() << tmp_path << ":" << QString::fromStdString(name) ;
                avaliableJoysticks.insert( cpt , js.get() ) ;
            }
        }
        else 
        {
            next_joystick = false;
        }        
        cpt++ ;
    }
    
    // No valid joystick: abort !
    if ( avaliableJoysticks.isEmpty() ) 
    {
        QMessageBox::critical( 0 , QObject::tr("Joystick mapper") , QObject::tr("No valid joystick found!") ) ;
        return EXIT_FAILURE ;
    }
    
    
    // Translations
//     QString locale = QLocale::system().name().section("_",0,0);   
//     
//     QTranslator translator;
//     translator.load(QString(":/lang/") + locale);
//     app.installTranslator(&translator);
//     
//     QTranslator translatorQt;
//     translatorQt.load(QString("qt_") + locale ,  QLibraryInfo::location(QLibraryInfo::TranslationsPath) );
//     app.installTranslator(&translatorQt);
    
    
    
    // Window :
    mogs::JoystickMapper mapper ;
    
    
    
    mapper.show();
    
    return app.exec() ;
}