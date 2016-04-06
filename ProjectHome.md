_looking for the [old web page](http://lego.bldesign.org/LSculpt/index_old.html)?_

# LSculpt: Studs-out LEGO® Sculpture #
LSculpt is a program which converts a triangle mesh into an LDraw file. Like the [sphere generator](http://lego.bldesign.org/sphere/), the generated model consists of a surface of 1×1 plates oriented in whichever direction provides the best detail.

Here's an example sculpture designed with LSculpt.
![http://lego.bldesign.org/LSculpt/bunnies_624.jpg](http://lego.bldesign.org/LSculpt/bunnies_624.jpg)

## download ##
LSculpt is currently available for Windows.  You can download the latest version [here](http://code.google.com/p/lsculpt/downloads).

Interested developers may also take a look at the [source code](http://code.google.com/p/lsculpt/source).  LSculpt is written in C++.  The graphical interface uses the [Qt Creator](http://qt.nokia.com/) application framework.  The preview window is powered by Travis Cobbs' [LDView](http://ldview.sourceforge.net/).


## screenshots ##
![http://lego.bldesign.org/LSculpt/ls_screen_mesh.png](http://lego.bldesign.org/LSculpt/ls_screen_mesh.png)

![http://lego.bldesign.org/LSculpt/ls_screen_lego.png](http://lego.bldesign.org/LSculpt/ls_screen_lego.png)


## working with 3D models ##

### finished models ###
The bunny model shown on these pages is from the [Stanford 3D Scanning Repository](http://graphics.stanford.edu/data/3Dscanrep/).  A great list of models and model repositories is [available here](http://lodbook.com/models/).  Happy hunting!

### converting between 3D formats ###
LSculpt currently supports most [PLY](http://en.wikipedia.org/wiki/PLY_%28file_format%29), [STL](http://en.wikipedia.org/wiki/STL_%28file_format%29), and [OBJ](http://en.wikipedia.org/wiki/Obj) files.  If you come across other formats, you will need to convert them.
  * [MeshLab](http://meshlab.sourceforge.net/), a free program for processing, editing, and converting 3D meshes
  * [3D Object Converter](http://web.axelero.hu/karpowh/), a graphical conversion utility between many 3D formats
  * [IVCON](http://lego.bldesign.org/LSculpt/ivcon.zip) (149 KB) Windows command line utility that converts between many 3D formats _([source](http://people.scs.fsu.edu/~burkardt/cpp_src/ivcon/ivcon.html))_.

### creating your own models ###
Many programs can create files that will work with LSculpt.
  * Most [3D computer aided design (CAD) software](http://en.wikipedia.org/wiki/Comparison_of_3D_computer_graphics_software) can export to STL.
  * [DAZ 3D](http://www.daz3d.com/) and [Poser](http://poser.smithmicro.com/poser.html) can export Wavefront OBJ files.

_[LEGO®](http://lego.com/) is a registered trademark of the LEGO Group, which does not sponsor, endorse, or authorize this software project._