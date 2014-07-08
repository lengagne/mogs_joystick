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
#include "JoystickSelector.h"
#include "Joystick.h"

int main (int argc, char *argv[])
{
    QApplication app( argc , argv ) ;
    
    // First, we lists the avaliable joypads and see what we can do :
    QString portToUse ;
    mogs::JoystickSelector jsSelect ;
    jsSelect.findValidJoysticks();
   
    // No valid joystick: abort !
    if ( jsSelect.jsCount() == 0 ) 
    {
        QMessageBox::critical( 0 , QObject::tr("Joystick mapper") , QObject::tr("No valid joystick found!") ) ;
        return EXIT_FAILURE ;
    }
    else if ( jsSelect.jsCount() == 1 ) 
    {
        portToUse = jsSelect.selectedPort() ;
    } 
    else 
    {
        int ret = jsSelect.exec() ;
        if ( ret == QDialog::Accepted ) 
            portToUse = jsSelect.selectedPort() ;
        else 
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
    mapper.setJoystick( portToUse ) ;
    mapper.show();
    
    return app.exec() ;
}