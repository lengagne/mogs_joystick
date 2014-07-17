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
#include "MoGS_Config_Joystick.h"

#include <climits>
#include <vector>
#include <QDebug>
#include <QMetaEnum>

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

using namespace mogs ;

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
            new jsItem(r,axisItem) ;
        endInsertColumns();
        endInsertRows() ;
        emit dataChanged( index(0,0,axisIndex()) , index(js->axes,2,axisIndex()) );
        
        // button
        beginInsertRows( buttonIndex() , 0 , js->buttons );
        beginInsertColumns( buttonIndex() , 0 , 2 );
        for ( int r = 0 ; r < (int)js->buttons ; r++ ) 
            new jsItem(r,buttonItem) ;
        endInsertColumns();
        endInsertRows() ;
        emit dataChanged( index(0,0,buttonIndex()) , index(js->buttons,2,buttonIndex()) );
        
        emit layoutChanged();
    }
    
    readConfig(); 
    
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
            return 4 ;
        else 
            return 1 ;
    }

}

/*!
 * 
 */
QVariant JoystickWrapper::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( !validRole(role) ) 
        return QVariant() ;
    
    if ( orientation == Qt::Horizontal && section == 0 ) 
        return QString("id") ;
    else if ( orientation == Qt::Horizontal &&  section == 1 ) 
        return QString("Value") ;
    else if ( orientation == Qt::Horizontal &&  section == 2 ) 
        return QString("Action") ;
    else if ( orientation == Qt::Horizontal &&  section == 3 ) 
        return QString("Inverted axes") ;
    else if ( orientation == Qt::Vertical ) 
        return section ;
    
    return QAbstractItemModel::headerData(section, orientation, role);
}


/*!
 * 
 */
QVariant JoystickWrapper::data( const QModelIndex& index , int role ) const
{
    if ( !js || !index.isValid() ) 
        return QVariant() ;
    
    if ( !validRole(role) )
        return QVariant();
    
    jsItem * item = getItem(index) ;
    if ( item == axisItem  && role == Qt::DisplayRole ) 
        return QString("axes") ;
    else if ( item == buttonItem && role == Qt::DisplayRole ) 
        return QString("buttons") ;
    else 
    {
        if ( index.column() == 0 && role == Qt::DisplayRole ) 
        {
            QString str ;
            if ( getItem(index.parent()) == axisItem )
                str = "axis %1" ;
            else 
                str = "button %1" ;
                    
            return str.arg(index.row()) ;
        }
        else if ( index.column() == 1 && role == Qt::DisplayRole ) 
            return QVariant::fromValue<int16_t>( item->value ) ;
        else if ( index.column() == 1 && role == Qt::CheckStateRole ) 
            return QVariant::fromValue<bool>( item->inverted ) ;
        else if ( index.column() == 2 && role == Qt::DisplayRole )
            return QVariant::fromValue<int>( item->action ) ;
        else if ( index.column() == 2 && role == Qt::EditRole )
        {
            QStringList actions ; 
            if ( getItem(index.parent()) == axisItem )
                actions = getAxisActions() ;
            else 
                actions = getButtonActions() ;
            
            return QVariant::fromValue<QStringList>(actions) ;
        }
        else if ( index.column() == 3 && role == Qt::CheckStateRole )
            return (item->inverted)? Qt::Checked : Qt::Unchecked ;
        else if ( index.column() == 3 && role == Qt::DisplayRole )
            return QString( "Inverted axis" ) ;
            
    }
       
    return QVariant() ;
}

/*!
 * 
 */
bool JoystickWrapper::setData(const QModelIndex & index, const QVariant& value, int role)
{
    if ( !index.isValid() ) 
        return false ;
    
    jsItem * item = getItem( index ) ;
    if ( index.column() == 2 && Qt::EditRole )
    {
        item->action = value.toInt() ;
        emit dataChanged(index,index);
        return true ;
    }
    else if ( index.column() == 3 && Qt::CheckStateRole )
    {
        item->inverted = value.toBool() ;
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
        else if ( index.column() == 3 )
            return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
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

/*!
 * 
 */
bool JoystickWrapper::validRole(const int& role) const
{
    if ( role == Qt::EditRole ) 
        return true ;
    
    if ( role == Qt::DisplayRole )
        return true ;
    
    if ( role == Qt::CheckStateRole )
        return true ;
    
    if ( role >= Qt::UserRole )
        return true ;
    
    
    return false ;
}

/*!
 * 
 */
void JoystickWrapper::readConfig()
{
    MoGS_Config_Joystick config_finder;

    if ( !config_finder.read_config() )
        config_finder.create_config();
    
    // Reset values 
    for ( int n = 0 ; n < rowCount(axisIndex()) ; n++ )
    {
        setData( index(n,2,axisIndex()) , QVariant::fromValue<int>( NoAxisAction ) , Qt::EditRole ) ;
        setData( index(n,3,axisIndex()) , QVariant::fromValue<bool>( false ) , Qt::EditRole ) ;
    }

    for ( int n = 0 ; n < rowCount(buttonIndex()) ; n++ )
    {
        setData( index(n,2,buttonIndex()) , QVariant::fromValue<int>( NoButtonAction ) , Qt::EditRole ) ;
    }
    
    // Update from config file if exits
    if ( config_finder.has_pad(js->name) )
    {
        pad_control conf = config_finder.get_pad_config( js->name ) ;
        
        // Forward action
        if ( conf.forward.push == AXIS ) 
        {
            setData( index(conf.forward.id,2,axisIndex()) , QVariant::fromValue<int>( AxisFrontBack ) , Qt::EditRole ) ;
            setData( index(conf.forward.id,3,axisIndex()) , QVariant::fromValue<bool>( conf.forward.sign == -1.0 ) , Qt::EditRole ) ;
        }
        else if ( conf.forward.push == BUTTON ) 
        {
            setData( index(conf.forward.id,2,buttonIndex()) , QVariant::fromValue<int>(ButtonFront) , Qt::EditRole ) ;
            setData( index(conf.forward.id_neg,2,buttonIndex()) , QVariant::fromValue<int>(ButtonBack) , Qt::EditRole ) ;
        }
        
        // Side action
        if ( conf.side.push == AXIS ) 
        {
            setData( index(conf.side.id,2,axisIndex()) , QVariant::fromValue<int>( AxisLeftRight ) , Qt::EditRole ) ;
            setData( index(conf.side.id,3,axisIndex()) , QVariant::fromValue<bool>( conf.side.sign == -1.0 ) , Qt::EditRole ) ;
        }
        else if ( conf.side.push == BUTTON ) 
        {
            setData( index(conf.side.id,2,buttonIndex()) , QVariant::fromValue<int>(ButtonLeft) , Qt::EditRole ) ;
            setData( index(conf.side.id_neg,2,buttonIndex()) , QVariant::fromValue<int>(ButtonRight) , Qt::EditRole ) ;
        }
        
        // Up
        if ( conf.up.push == AXIS ) 
        {
            setData( index(conf.up.id,2,axisIndex()) , QVariant::fromValue<int>( AxisUpDown ) , Qt::EditRole ) ;
            setData( index(conf.up.id,3,axisIndex()) , QVariant::fromValue<bool>( conf.up.sign == -1.0 ) , Qt::EditRole ) ;
        }
        else if ( conf.up.push == BUTTON ) 
        {
            setData( index(conf.up.id,2,buttonIndex()) , QVariant::fromValue<int>(ButtonUp) , Qt::EditRole ) ;
            setData( index(conf.up.id_neg,2,buttonIndex()) , QVariant::fromValue<int>(ButtonDown) , Qt::EditRole ) ;
        }
        
        // Rotate Action 
        if ( conf.rotate.push == AXIS ) 
        {
            setData( index(conf.rotate.id,2,axisIndex()) , QVariant::fromValue<int>( AxisRotate ) , Qt::EditRole ) ;
            setData( index(conf.rotate.id,3,axisIndex()) , QVariant::fromValue<bool>( conf.rotate.sign == -1.0 ) , Qt::EditRole ) ;
        }
        else if ( conf.rotate.push == BUTTON ) 
        {
            setData( index(conf.rotate.id,2,buttonIndex()) , QVariant::fromValue<int>(ButtonRotateLeft) , Qt::EditRole ) ;
            setData( index(conf.rotate.id_neg,2,buttonIndex()) , QVariant::fromValue<int>(ButtonRotateRight) , Qt::EditRole ) ;
        }
        
        // Start/Stop 
        setData( index(conf.pause_button.id,2,buttonIndex()) , QVariant::fromValue<int>(ButtonStart) , Qt::EditRole ) ;
        setData( index(conf.stop_button.id,2,buttonIndex()) , QVariant::fromValue<int>(ButtonStop) , Qt::EditRole ) ;
        
    }

}

/*!
 * 
 */
void JoystickWrapper::saveConfig()
{
    // Get existing config
    MoGS_Config_Joystick config_finder ;
    config_finder.read_config() ;
    
    pad_control config ;
    if ( config_finder.has_pad( js->name ) ) 
        config = config_finder.get_pad_config( js->name ) ;
    else 
        config.name = name().toStdString() ;
    
    // Read buttons ;
    foreach( jsItem * button , buttonItem->childs )
    {
        switch( button->action )
        {
        case ( ButtonFront ) :
        {
            config.forward.push = BUTTON ;
            config.forward.id = button->row ;
            break ;
        }

        case ( ButtonBack ) :
        {
            config.forward.push = BUTTON ;
            config.forward.id_neg = button->row ;
            break ;
        }

        case ( ButtonLeft ) :
        {
            config.side.push = BUTTON ;
            config.side.id = button->row ;
            break ;
        }

        case ( ButtonRight ) :
        {
            config.side.push = BUTTON ;
            config.side.id_neg = button->row ;
            break ;
        }
        
        case ( ButtonUp ) :
        {
            config.up.push = BUTTON ;
            config.up.id = button->row ;
            break ;
        }

        case ( ButtonDown ) :
        {
            config.up.push = BUTTON ;
            config.up.id_neg = button->row ;
            break ;
        }

        case ( ButtonRotateLeft ) :
        {
            config.rotate.push = BUTTON ;
            config.rotate.id = button->row ;
            break ;
        }

        case ( ButtonRotateRight ) :
        {
            config.rotate.push = BUTTON ;
            config.rotate.id_neg = button->row ;
            break ;
        }

        case ( ButtonStop ) :
        {
            config.stop_button.id = button->row ;
            break ;
        }

        case ( ButtonStart ) :
        {
            config.pause_button.id = button->row ;
            break ;
        }

        case ( NoButtonAction ) :
        default :
            break ;

        }
    }
    
    foreach( jsItem * axis , axisItem->childs )
    {
        switch ( axis->action )
        {
        case( AxisFrontBack ) :
        {
            config.forward.push = AXIS ;
            config.forward.id = axis->row ;
            config.forward.sign = (axis->inverted)? -1.0 : 1.0 ;
            break ;
        }

        case( AxisLeftRight ) :
        {
            config.side.push = AXIS ;
            config.side.id = axis->row ;
            config.side.sign = (axis->inverted)? -1.0 : 1.0 ;
            break ;
        }
        
        case( AxisUpDown ) :
        {
            config.up.push = AXIS ;
            config.up.id = axis->row ;
            config.up.sign = (axis->inverted)? -1.0 : 1.0 ;
            break ;
        }

        case( AxisRotate ) :
        {
            config.rotate.push = AXIS ;
            config.rotate.id = axis->row ;
            config.rotate.sign = (axis->inverted)? -1.0 : 1.0 ;
            break ;
        }

        case ( NoAxisAction ) :
        default :
            break ;

        } ;
    }
    
    config_finder.add_Joystick( config );
    config_finder.save_config() ;
    
}

/*!
 * 
 */
bool JoystickWrapper::actionIsSet( const JoystickWrapper::AxisActions& action ) const
{
    if ( action == NoAxisAction ) 
        return false ;
    
    foreach( jsItem * axis , axisItem->childs )
    {
        if ( axis->action == action ) 
            return true ;
    }
    
    foreach( jsItem * button , buttonItem->childs )
    {
        switch( action )
        {
            case( AxisFrontBack ) : 
                if ( button->action == ButtonFront || button->action == ButtonBack ) return true ;
                break ;
            case( AxisLeftRight ) : 
                if ( button->action == ButtonLeft || button->action == ButtonRight ) return true ;
                break ;
            case( AxisUpDown ) : 
                if ( button->action == ButtonUp || button->action == ButtonDown ) return true ;
                break ;
            case( AxisRotate ) : 
                if ( button->action == ButtonRotateLeft || button->action == ButtonRotateRight ) return true ;
                break ;
            case( NoAxisAction ) :
            default :
                break ;
        }
    }
    
    return false ;
}

/*!
 * 
 */
bool JoystickWrapper::actionIsSet( const JoystickWrapper::ButtonActions& action ) const
{
    if ( action == NoButtonAction )
        return false ;
    
    foreach( jsItem * button , buttonItem->childs ) 
    {
        if ( button->action == action ) 
            return true ;
    }
    
    foreach ( jsItem * axis , axisItem->childs )
    {
        switch( action ) 
        {
            case ( ButtonFront ) :
            case ( ButtonBack ) :
                if ( axis->action == AxisFrontBack ) return true ;
                break ;
            case ( ButtonLeft ) :
            case ( ButtonRight ) :
                if ( axis->action == AxisLeftRight ) return true ;
                break ;
            case ( ButtonUp ) :
            case ( ButtonDown ) :
                if ( axis->action == AxisUpDown ) return true ;
                break ;
            case ( ButtonRotateLeft ) :
            case ( ButtonRotateRight ) :
                if ( axis->action == AxisRotate ) return true ;
                break ;
            case ( ButtonStop ) :
            case ( ButtonStart ) :
                break ;
            case ( NoButtonAction ) :
            default :
                break ;
        }
    }
    
    return false ;
}

/*!
 * 
 */
QStringList JoystickWrapper::getAxisActions() const
{
    const QMetaObject &mo = JoystickWrapper::staticMetaObject ;
    int eIdx = mo.indexOfEnumerator( "AxisActions" ) ;
    QMetaEnum e = mo.enumerator(eIdx) ;
    
    QStringList actions ;
    for( int n = 0 ; n < e.keyCount() ; n++ )
    {
        QString actTxt = e.key(n) ;
        int actVal = e.value(n) ;
        
        if ( !actionIsSet( static_cast<AxisActions>(actVal) ) ) 
        {
            actions << actTxt ;
        }
    }
    
    return actions ;
}

/*!
 * 
 */
QStringList JoystickWrapper::getButtonActions() const
{
    const QMetaObject &mo = JoystickWrapper::staticMetaObject ;
    int eIdx = mo.indexOfEnumerator( "ButtonActions" ) ;
    QMetaEnum e = mo.enumerator(eIdx) ;
    
    QStringList actions ;
    for( int n = 0 ; n < e.keyCount() ; n++ )
    {
        QString actTxt = e.key(n) ;
        int actVal = e.value(n) ;
        
        if ( !actionIsSet( static_cast<ButtonActions>(actVal) ) ) 
        {
            actions << actTxt ;
        }
    }
    
    return actions ;
}

