/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2014  ClÃ©ment Fouque <email>
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

#include "JoystickWrapper.h"
#include "Joystick.h"
#include "JoystickConfigurationModel.h"
#include "AxisRangeDelegate.h"

#include <memory>
#include <climits>

#include <QVBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QLayout>
#include <QListView>

using namespace mogs ;

/*!
 * 
 */
JoystickWrapper::JoystickWrapper( QWidget * parent) :
    QWidget(parent) ,
    js(0)
{
    this->setLayout( new QVBoxLayout(this) );
}

/*!
 * 
 */
JoystickWrapper::~JoystickWrapper()
{
    
}

/*!
 * 
 */
void JoystickWrapper::setJoystick( JoystickConfigurationModel * js )
{   
//     js = js_ ;
//     if ( js->active )
//     {
        QListView * view = new QListView(this) ;
	layout()->addWidget(view);
	
	view->setModel( js );
	view->setRootIndex(js->axisIndex());
	view->setModelColumn(1);
	view->setBatchSize(1);
	view->setLayoutMode( QListView::Batched );
	view->setItemDelegate( new AxisRangeDelegate(this) );
//         for ( int n = 0 ; n < (int)js->axes ; n++ ) 
//         {
//             QSlider * viz = new QSlider( Qt::Horizontal , this) ;
//             viz->setRange( SHRT_MIN , SHRT_MAX ) ;
//             viz->setValue( 0 ) ;
//             viz->setTextVisible(false);
//             m_axisState.insert( n , viz ) ;
//             layout()->addWidget( viz );
//         }
/*        
        for ( int n = 0 ; n < (int)js->buttons ; n++ ) 
        {
            QPushButton * b = new QPushButton( QString::number(n) , this ) ;
            b->setMaximumSize( QSize(50,50) );
            b->setCheckable( true ) ;
            m_buttonsState.insert( n , b ) ;
            layout()->addWidget(b) ;
        }
        
        startTimer(100) ;
    }*/
}

/*!
 * 
 */
void JoystickWrapper::timerEvent( QTimerEvent * event )
{
    for ( int n = 0 ; n < (int)js->axes ; n++ ) 
    {
        m_axisState.value(n)->setValue( js->joystick_st->axis[n] ) ;
    }
    
    for ( int n = 0 ; n < (int)js->buttons ; n++ ) 
    {
        m_buttonsState.value(n)->setChecked( js->joystick_st->button[n] ) ;
    }
}

/*!
 * 
 */
