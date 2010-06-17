/*	LSculpt: Studs-out LEGO® Sculpture

	Copyright (C) 2010 Bram Lambrecht <bram@bldesign.org>

	http://lego.bldesign.org/LSculpt/

	This file (main.cpp) is part of LSculpt.

	LSculpt is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	LSculpt is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see http://www.gnu.org/licenses/  */

#ifndef LSCULPT_CONSOLE

#include <QApplication>
#include "lsculptmainwin.h"
#include <exception>

void handler()
{
	cout << "Fatal Error!\n";
	//exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	set_terminate(handler);
	QApplication a(argc, argv);
	LSculptMainWin w;
	w.show();
	return a.exec();
}
#endif  // LSCULPT_CONSOLE
