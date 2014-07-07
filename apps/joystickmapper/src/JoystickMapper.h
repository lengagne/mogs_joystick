/*
 * Configuration interface for mogs joysticks
 * 
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

#ifndef MOGS_JOYSTICKMAPPER_H
#define MOGS_JOYSTICKMAPPER_H

#include <QMainWindow>

namespace Ui
{
class JoystickMapper;
}

namespace mogs 
{

class JoystickMapper : public QMainWindow 
{
    Q_OBJECT
    
    Ui::JoystickMapper * ui;
    
public:
    explicit JoystickMapper();
    virtual ~JoystickMapper();

};

}

#endif // MOGS_JOYSTICKMAPPER_H
