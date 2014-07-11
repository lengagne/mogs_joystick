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

#include "JoystickActionDelegate.h"
#include "JoystickWrapper.h"

#include <QComboBox>
#include <QMetaEnum>
#include <QDebug>
#include <QSpinBox>
#include <QPainter>
#include <QApplication>

/*!
 * 
 */
JoystickActionDelegate::JoystickActionDelegate(const QString& enumName , QObject * parent ) :
    QItemDelegate(parent)
{
    const QMetaObject &mo = JoystickWrapper::staticMetaObject ;
    int eIdx = mo.indexOfEnumerator( enumName.toAscii() ) ;
    m_enum = mo.enumerator(eIdx) ;
}

/*!
 * 
 */
JoystickActionDelegate::~JoystickActionDelegate()
{
    
}

/*!
 * Create the combo box
 */
QWidget* JoystickActionDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const
{
    QComboBox * editor = new QComboBox(parent) ;
    
    for( int n = 0 ; n < m_enum.keyCount() ; n++ )
    {
        QString actTxt = m_enum.key(n) ;
        int actVal = m_enum.value(n) ;
        editor->addItem( actTxt , actVal );
    }
    
    editor->setAutoFillBackground(true);
//     editor->move(option.rect.x(),option.rect.y());
//     editor->showPopup();
    qDebug() << editor << editor->isVisible() ;
    
    return editor ;
}

/*!
 * set combobox value from model
 */
void JoystickActionDelegate::setEditorData( QWidget * editor, const QModelIndex& index ) const
{
    QComboBox * cb = qobject_cast<QComboBox*>( editor ) ;
    if ( cb )
    {
        int i = cb->findData( index.data() ) ;
        cb->setCurrentIndex(i);
    }
    else
        QItemDelegate::setEditorData(editor,index) ;

}

/*!
 * set Model value from editor 
 */
void JoystickActionDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox * cb = qobject_cast<QComboBox*>( editor ) ;
    if ( cb )
    {
        QVariant act = cb->itemData( cb->currentIndex() ) ;
        model->setData(index,act,Qt::EditRole) ;
    }
    else
        QItemDelegate::setModelData(editor, model, index);
}

/*!
 * 
 */
void JoystickActionDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const
{
    qDebug() << option.rect ;
    editor->setGeometry( option.rect );
}

/*!
 * 
 */
void JoystickActionDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
//     bool isSelected = option.state & QStyle::State_Selected ;
//     if ( isSelected ) 
//     {
//         painter->drawRect( option.rect , );
//     }
//     
    int act = index.data().toInt() ;
    QString actTxt = m_enum.key(act) ;
    painter->drawText( option.rect , option.displayAlignment , actTxt );
//     QApplication::style()->drawComplexControl( QStyle::CC_ComboBox , &option, painter , 0 );
}
