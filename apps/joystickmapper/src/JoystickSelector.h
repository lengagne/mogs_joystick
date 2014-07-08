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

#ifndef __MOGS_JOYSTICKSELECTOR_H__
#define __MOGS_JOYSTICKSELECTOR_H__

#include <QDialog>

class QDataWidgetMapper;
class QStandardItemModel ;
namespace Ui
{
class JoystickSelector;
}

namespace mogs 
{

class JoystickSelector : public QDialog
{
    Q_OBJECT
    Q_PROPERTY( int selectedPort READ selectedPort WRITE setSelectedPort ) ;

    Ui::JoystickSelector * ui;
    QStandardItemModel * m_avJs ;
    
public:
    explicit JoystickSelector( QWidget * parent = 0 );
    virtual ~JoystickSelector();
    int selectedPort() const ;
    int jsCount() const ;
    
public slots :
    void setSelectedPort( const int & port ) ;
    void findValidJoysticks() ;

private :
    void initializeModel() ;
    
} ;

} ;

#endif // MOGS_JOYSTICKSELECTOR_H
