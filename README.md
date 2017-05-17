# APO-LAU
Apo lau is semestral project for APO on CTU. LAU stands for Light admin unit.
Goal of semestral project was to create C/CPP program which handles lights in individual rooms, communicates with other devices and displays data to display.
## Usage ##
- download this project
- `make`
- `./app conf_files/unit1.txt`

## Info ##
Project is split up into individual files. Goal was to keep as many files as possible in pure C. Things that I found too hard or I was too lazy to write in C are written in CPP.
You will find following files in the project:
- light_admin_unit - light admin unit struct and related methods
- pixel - pixel struct and related methods
- mzapo_* - files provided by our teacher, methods and constants to access all peripherals
- font_* - files provided by our teacher, methods and structs with fonts
- socket_rocket - all stuff necessary for UDP LAN communication with other lau's, handles update sending and update receiving, dynamically creates list of all connected units
- console_info - displays everything that's happening into the console, mainly for debugging or if the unit doesn't have a display
- display - handles everything related to peripherals, from displaying data to reading input from knobs

#### Download ####
`git clone https://github.com/matoous/apo_lau`

`cd apo_lau/`

#### Compile ####

`make`

Apo-lau uses gcc and g++ for its compilation, feel free to change these but do not forget to add threads flag.

#### Run ####

`./app configuration_file`

Configuration file is used to load initial data of the unit. Configuration file is structured as follows:

`unit name`  // unit name on separate line, the first line, required

`ceiling color r g b` // 3 integer values => 0 and <= 256 defining the color of ceiling light, required

`walls color r g b` // 3 integer values => 0 and <= 256 defining the color of walls light, required

`icon 256*uint16_t` // 256 uint16_t values defining the 16x16 icon of the unit in rgb565 format, required

To safely stop the application all you need is to press any button, all the threads one by one will get killed
and the application will end.