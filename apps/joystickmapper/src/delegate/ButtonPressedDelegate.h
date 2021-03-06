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

#ifndef __MOGS_BUTTONPRESSEDDELEGATE_H__
#define __MOGS_BUTTONPRESSEDDELEGATE_H__

#include <QStyledItemDelegate>

namespace mogs
{

class ButtonPressedDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ButtonPressedDelegate( QObject * parent = 0 );
    ~ButtonPressedDelegate();
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const ;
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const ;

};

};

#endif // BUTTONPRESSEDDELEGATE_H
