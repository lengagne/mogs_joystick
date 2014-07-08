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

#include "ActionConfigurationView.h"
#include "ui_ActionConfigurationView.h"
#include <QComboBox>

using namespace mogs;

/*!
 * Widget constructor
 */
ActionConfigurationView::ActionConfigurationView( QWidget * parent )
{
    ui = new Ui::ActionConfigurationView;
    ui->setupUi(this);
    
    ui->buttonBox->setEnabled(false);
}

/*!
 * 
 */
ActionConfigurationView::~ActionConfigurationView()
{
    delete ui;
}

/*!
 * 
 */
void ActionConfigurationView::resetUi(const int& nbAxis, const int& nbButtons)
{
    QStringList axisActions = getAxisActions() ;
    for ( int n = 0 ; n < nbAxis ; n++ ) 
    {
        const QString label = tr("Axis %1 :").arg(n) ;
        QComboBox * cb = new QComboBox( ui->axisBox ) ;
        cb->addItems( axisActions );
        static_cast<QFormLayout*>( ui->axisBox->layout() )->addRow( label , cb );
    }
    
    QStringList buttonsActions = getButtonsActions() ;
    for ( int n = 0 ; n < nbButtons ; n++ ) 
    {
        const QString label = tr("Button %1 :").arg(n) ;
        QComboBox * cb = new QComboBox( ui->buttonsBox ) ;
        cb->addItems( buttonsActions );
        static_cast<QFormLayout*>( ui->buttonsBox->layout() )->addRow( label , cb );
    }
    
}

/*!
 * List available actions.
 * Can be enhanced by parsing DTD file
 */
QStringList ActionConfigurationView::getAxisActions() const
{
    QStringList actions ; 
    actions << QString() ;
    actions << tr("Move forward/backward") ;
    actions << tr("Move left/right") ;
    actions << tr("Rotate left/right") ;
    return actions ;
}

/*!
 * List available actions.
 * Can be enhanced by parsing DTD file
 */
QStringList ActionConfigurationView::getButtonsActions() const
{
    QStringList actions ; 
    actions << QString() ;
    actions << tr("Start") ;
    actions << tr("Stop") ;
    actions << tr("Move forward") ;
    actions << tr("Move backward") ;
    actions << tr("Move left") ;
    actions << tr("Move right") ;
    actions << tr("Rotate left") ;
    actions << tr("Rotate right") ;
    return actions ;
}
