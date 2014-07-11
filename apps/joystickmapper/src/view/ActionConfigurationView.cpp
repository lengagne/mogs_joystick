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

#include "JoystickWrapper.h"
#include "delegate/JoystickActionDelegate.h"

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
void ActionConfigurationView::setModel(JoystickWrapper* model)
{
    if ( !model ) return ;

    QStandardItemModel * m = new QStandardItemModel( 5,3 ) ;
    
    // Axes:
    JoystickActionDelegate * axesDelegate = new JoystickActionDelegate( "AxisActions" , ui->axisTableView ) ;
    ui->axisTableView->setModel(model) ;
    ui->axisTableView->setRootIndex(model->axisIndex()) ;
    ui->axisTableView->setItemDelegateForColumn( 2 , axesDelegate );
    ui->axisTableView->hideColumn(1);
    
    JoystickActionDelegate * buttDelegate = new JoystickActionDelegate( "ButtonActions" , ui->axisTableView ) ;
    ui->buttonTableView->setModel(model);
    ui->buttonTableView->setRootIndex(model->buttonIndex()) ;
    ui->buttonTableView->setItemDelegateForColumn(2,buttDelegate) ;
    ui->buttonTableView->hideColumn(1);
}
