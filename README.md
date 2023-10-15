# CPS511 A1 - Zeppelin Model
This project implements all required functionality and requirements as outlined in the CPS511 A1 Requirements Document. The model has been constructed hierarchically and is 
composed of 10 total parts with over 3 levels of hierarchy. The zeppelin model can be moved forward via the w key and turned left/right using the left/right arrow keys and 
moved vertically up/down via the up/down arrow keys. The propeller blades have been made to animate and spin by default as soon as the model is rendered. 

## Compilation Note: 
This project was built and compiled on Mac OS, as such the associated makefile has only been tested on a Mac Machine (OS X) and I cannot guarantee its function on other 
platforms. The linked and imported libraries have also only been tested on Mac. If you are looking to compile and run this program (Zeppelin.cpp) on another platform please 
adjust the imported libraries where necessary and compile using the method that works for your platform.

## Model Parts
The Zeppelin Model is composed of the following 10 Parts:
- Zeppelin Body
- Zeppelin Fins (Top, Right, and Left Fins)
- Zeppelin Command Center (Cabin)
- Body Drive Shaft
- Body Propeller Blades
- Command Center (Cabin) Drive Shaft
- Command Center (Cabin) Propeller Blades
- Undercarriage Light Base
- Undercarriage Left Light
- Undercarriage Right Light

## Model Hierarchy
Body
|-- Top Fin
|-- Right Fin
|-- Left Fin
|-- Command Center (Cabin)
|   |-- Drive Shaft
|   |   `-- Propeller Blades
|   `-- Light Base
|       |-- Left Light
|       `-- Right Light
`-- Body Drive Shaft
    `-- Body Propeller Blades


