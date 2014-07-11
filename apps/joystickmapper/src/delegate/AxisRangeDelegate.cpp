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

#include "AxisRangeDelegate.h"

#include <climits>
#include <QPainter>
#include <QDebug>

using namespace mogs ;

/*!
 * 
 */
AxisRangeDelegate::AxisRangeDelegate( QObject * parent ) :
  QStyledItemDelegate(parent) , 
  max_v( (qreal)SHRT_MAX ) ,
  min_v( (qreal)SHRT_MIN )
{
    
  
}

/*!
 * 
 */

AxisRangeDelegate::~AxisRangeDelegate()
{
  
}

/*!
 * 
 */
QSize AxisRangeDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(75,25) ;
//     return QStyledItemDelegate::sizeHint(option, index);
}

/*!
 * 
 */
void AxisRangeDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex & index) const
{
   QBrush bg = option.palette.dark() ;
   painter->setRenderHint(QPainter::HighQualityAntialiasing);
   
   // bg
   painter->setPen( Qt::NoPen );
   painter->setBrush(bg);
   int r = option.rect.height()/2 ;
   painter->drawRoundedRect( option.rect , r , r );
   
   // Current pos :
   qreal p = computeRelativePosition( index ) ;
   QRect cursorRect ;
   int length = option.rect.width() - 2*r ;
   cursorRect.setTopLeft( option.rect.topLeft() );
   cursorRect.setHeight( option.rect.height() );
   cursorRect.setWidth( option.rect.height() );
   cursorRect.translate( p * length , 0 );
   painter->setBrush( Qt::darkRed );
   painter->setPen( Qt::black );
   painter->drawEllipse( cursorRect );
   
   QFont f = option.font ;
   f.setBold(true);
   painter->setPen(Qt::white);
   painter->setFont(f);
   painter->drawText( cursorRect , Qt::AlignCenter , QString::number(index.row()));
    
}

/*!
 * 
 */
qreal AxisRangeDelegate::computeRelativePosition(const QModelIndex& index) const
{
    qreal v = index.data().toReal() ;
    qreal p = (v-min_v) / (max_v-min_v) ;
    bool inverted = index.data(Qt::UserRole).toBool() ;
    return (inverted)? (1.0-p) : p ;
}

