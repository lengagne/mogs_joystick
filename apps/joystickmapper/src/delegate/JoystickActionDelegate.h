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

#ifndef JOYSTICKACTIONDELEGATE_H
#define JOYSTICKACTIONDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QMetaEnum>

class QPainter ;

class JoystickActionDelegate : public QItemDelegate 
{
    Q_OBJECT
    QMetaEnum m_enum ;

public:
    explicit JoystickActionDelegate( const QString & enumName , QObject * parent = 0 );
    virtual ~JoystickActionDelegate();
    virtual QWidget * createEditor( QWidget * parent , const QStyleOptionViewItem & option, const QModelIndex & index ) const ;
    virtual void setEditorData( QWidget * editor , const QModelIndex & index ) const ; 
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const; 
    virtual void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

};

#endif // JOYSTICKACTIONDELEGATE_H
