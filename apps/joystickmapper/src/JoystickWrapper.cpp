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

#include "JoystickWrapper.h"
#include "Joystick.h"

#include <climits>
#include <vector>
#include <QDebug>

class jsItem 
{
public :
    jsItem( const int & r , jsItem * parent = 0 ) ;
    ~jsItem() ;
    
    QList<jsItem*> childs ;
    jsItem * parent ;
    int row ;
    int action ;
    short value ;
    bool inverted ;
} ;

jsItem::jsItem(const int& r, jsItem * p) :
    parent(p) ,
    row(r) ,
    action(0) ,
    value(0) , 
    inverted(false)
{
    if ( parent ) 
        parent->childs.push_back( this );
}

// destructor : delete childs !
jsItem::~jsItem()
{
    while ( !childs.isEmpty() )
    {
        delete childs.takeLast() ; 
    }
}



/*!
 * Constructor
 */
JoystickWrapper::JoystickWrapper(QObject* parent) :
    QAbstractItemModel(parent) ,
    axisItem(0) ,
    buttonItem(0)
{
    
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
bool JoystickWrapper::openDevice( const QString & device )
{
    cJoystick * js_ = new cJoystick ;
        
    const bool ok = js_->init(device.toAscii()) ;
    if ( ok ) 
    {
        emit layoutAboutToBeChanged() ;
        js = QSharedPointer<cJoystick>(js_) ;
        
        // Top items
        beginInsertRows( QModelIndex() , 0 , 1 );
        axisItem = new jsItem(0) ;
        buttonItem = new jsItem(1) ;
        endInsertRows();
        emit dataChanged( axisIndex() , buttonIndex() );
        
        
        // Axes items 
        beginInsertRows( axisIndex() , 0 , js->axes );
        beginInsertColumns( axisIndex() , 0 , 2 );
        for ( int r = 0 ; r < (int)js->axes ; r++ ) 
        {
            jsItem * a = new jsItem(r,axisItem) ;
        }
        endInsertColumns();
        endInsertRows() ;
        emit dataChanged( index(0,0,axisIndex()) , index(js->axes,2,axisIndex()) );
        
        // button
        beginInsertRows( buttonIndex() , 0 , js->buttons );
        beginInsertColumns( buttonIndex() , 0 , 2 );
        for ( int r = 0 ; r < (int)js->buttons ; r++ ) 
        {
            jsItem * b = new jsItem(r,buttonItem) ;
        }
        endInsertColumns();
        endInsertRows() ;
        emit dataChanged( index(0,0,buttonIndex()) , index(js->buttons,2,buttonIndex()) );
        
        emit layoutChanged();
    }
    
    return ok ;
}

/*!
 * Return item associated with the index
 */
jsItem * JoystickWrapper::getItem(const QModelIndex& index) const
{
    if ( !js ) return 0 ;
    
    if ( index.isValid() )
    {
       jsItem * item = static_cast<jsItem*>( index.internalPointer() ) ;
       return item ;
    }
    else 
        return 0 ;
}


/*!
 * 
 */
QModelIndex JoystickWrapper::index( int row , int column , const QModelIndex & parent) const
{
    if ( !js ) return QModelIndex() ;
    
    if (parent.isValid() && parent.column() != 0)
         return QModelIndex();   
    
    jsItem * parentItem = getItem(parent) ;
    
    if ( parentItem == 0 ) // looking for top items 
    {
        return (!row)? axisIndex() : buttonIndex() ;
    }
    else  // loking for child item
    {
        if ( parentItem->childs.isEmpty() ) 
            return QModelIndex() ;
        else 
        {             
            jsItem * childItem = parentItem->childs.at( row ) ;
            return createIndex( row , column , childItem ) ;
        }
    }
    
    return QModelIndex() ;
}

/*!
 * Get index of parent item
 */
QModelIndex JoystickWrapper::parent(const QModelIndex& child) const
{
    if ( !js ) 
        return QModelIndex() ;
    
    if (!child.isValid())
        return QModelIndex();

    jsItem * childItem = getItem(child);
    jsItem * parentItem = childItem->parent ;

    if ( !parentItem )
        return QModelIndex();
    else if ( parentItem == axisItem ) 
        return axisIndex() ;
    else if ( parentItem == buttonItem ) 
        return buttonIndex() ;

    return QModelIndex() ;
}

/*!
 * rowCount
 */
int JoystickWrapper::rowCount( const QModelIndex & parent ) const
{
    if ( !js ) 
        return 0 ;
    
    if ( !parent.isValid() ) // Model root
        return 2 ;
    else 
    {
        jsItem * parentItem = getItem(parent) ;
        return parentItem->childs.size() ;
    }
}

/*!
 * 
 */
int JoystickWrapper::columnCount(const QModelIndex& parent) const
{
    if (!js)
        return 0 ;
    
    if ( !parent.isValid() ) // Model root
        return 1 ;
    else 
    {
        jsItem * parentItem = getItem(parent) ;
        if ( !parentItem->childs.isEmpty() ) 
            return 3 ;
        else 
            return 1 ;
    }

}

/*!
 * 
 */
QVariant JoystickWrapper::data( const QModelIndex& index , int role ) const
{
    if ( !js || !index.isValid() ) 
        return QVariant() ;
    
    if (role != Qt::DisplayRole && role != Qt::EditRole )
        return QVariant();
    
    jsItem * item = getItem(index) ;
    if ( item == axisItem ) 
    {
        return QString("axes") ;
    }
    else if ( item == buttonItem ) 
    {
        return QString("buttons") ;
    }
    else 
    {
        if ( index.column() == 0 ) 
        {
            QString str ;
            if ( getItem(index.parent()) == axisItem )
                str = "axis %1" ;
            else 
                str = "button %1" ;
                    
            return str.arg(index.row()) ;
        }
        else if ( index.column() == 1 ) 
        {
            
            return QVariant::fromValue<int16_t>( item->value ) ;
        }
        else if ( index.column() == 2 ) 
        {
            return QVariant::fromValue<int>( item->action ) ;
        }
            
    }
       
    return QVariant() ;
}

/*!
 * 
 */
bool JoystickWrapper::setData(const QModelIndex & index, const QVariant& value, int role)
{
    if (role != Qt::EditRole)
        return false ;
    else if ( !index.isValid() || index.column() != 2 ) 
        return false ;
    
    jsItem * item = getItem( index ) ;
    if ( item ) 
    {
        item->action = value.toInt() ;
        emit dataChanged(index,index);
        return true ;
    }
    
    return QAbstractItemModel::setData(index,value,role) ;
}


/*!
 * 
 */
void JoystickWrapper::timerEvent(QTimerEvent* event)
{
    if ( !js && !js->active )
        return ;
    
    for( int n = 0 ; n < (int)js->axes ; n++ )
    {
        const short v = js->joystick_st->axis[n] ;
        if ( v != axisItem->childs[n]->value ) 
            axisItem->childs[n]->value = v ;
    }
    emit dataChanged( axisIndex() , axisIndex() ) ;
    
    for( int n = 0 ; n < (int)js->buttons ; n++ )
    {
        const short v = js->joystick_st->button[n] ;
        if ( v != buttonItem->childs[n]->value ) 
            buttonItem->childs[n]->value = v ;
    }
    emit dataChanged( buttonIndex() , buttonIndex() ) ;
    
//     QModelIndex a0 = index( 0 , 1 , axisIndex() ) ;
//     QModelIndex a1 = index( (int)js->axes , 1 , axisIndex() ) ;
//     emit dataChanged(a0,a1);
    
//     QModelIndex b0 = index( 0 , 1 , buttonIndex() ) ;
//     QModelIndex b1 = index( (int)js->axes , 1 , buttonIndex() ) ;
//     emit dataChanged(b0,b1);
}

/*!
 * 
 */
void JoystickWrapper::startAutoRefresh()
{
    if ( !js ) 
        return ;
    
    if ( !js->active ) 
        js->start() ;
    
    startTimer(50) ;
}

/*!
 * 
 */
QString JoystickWrapper::name() const
{
    if ( !js ) 
        return QString() ;
    else 
        return QString::fromStdString( js->name ) ;
}

/*!
 * 
 */
Qt::ItemFlags JoystickWrapper::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0 ;
    else 
    {
        if ( index.column() == 2 ) 
            return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
        else 
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
    }
}

/*!
 * 
 */
QModelIndex JoystickWrapper::axisIndex() const
{
    return createIndex(0,0,axisItem) ;
}

/*!
 * 
 */
QModelIndex JoystickWrapper::buttonIndex() const
{
    return createIndex(1,0,buttonItem) ;
}
