# CPS511 A3
This project implements all required functionality and requirements as outlined in the CPS511 A3 Requirements Document. 

Player and enemy zeppelins have been created and each have their bodies texture mapped, the fins of each of the zeppelins are read in meshes from A2 and have been textured mapped as well. 

The user can toggle between FPV and the normal camera using 't'.

Each zeppelin has a missile mounted under the cabin. The player can fire the missile using space bar and collisions are being detected using Bounding Boxes for each zeppelin. The Enemy zeppelin moves in a circular path until the player enters into a range around it and which point it turns and automatically fires a missile at the player.

If either the player or zeppelin is hit they will disappear, to respawn the player use 'r', to respawn the enemy use 'q'

The environment consists of a ground mesh created using the provided QuadMesh code from A1 that has been texture mapped with a road pattern. There are also buildings which have been created using a Cube class to generate buildings of various heights on either side of the road. Each building is texture mapped with a building exterior texture, except for the top face of the building. Lastly, there are a few vending machines on the road that are constructed similarly with cubes and texture mapped.

The enemy and player zeppelin have been restricted in their height so that collision with buildings are not possible.

## Video Demo:
The video demo begins with highlighting the movement and FPV functionality. The player then approaches the enemy and is fired at from the enemy zeppelin. When the missile intersects the player they are destroyed and disappear. The player is respawned using 'r' and then fires a missile at the enemy which when it intersects the enemy is destroyed and disappears. Finally the enemy is respawned and the player returns to the initial position. The ground, buildings, and vending machines can be seen in the environment.

## Close up Fin Image:
A Close up screenshot of the A2 mesh fin has been included for a better view of the object and texture mapping.

## Note on Performance:
Please note that the apparent slowness of the movement in the provided demo is a result of the hardware it was recorded on all functionality still works appropriately.

## Compilation Note: 
This project was built and compiled on Mac OS, as such the associated makefile has only been tested on a Mac Machine (OS X) and I cannot guarantee its function on other 
platforms. The linked and imported libraries have also only been tested on Mac. If you are looking to compile and run this program (Zeppelin.cpp) on another platform please 
adjust the imported libraries where necessary and compile using the method that works for your platform.

## Notes on Included Headers and Directories

### Objects & Textures
The objects and textures directories contain the mesh object files and texture images respectively that are used to construct the zeppelins and environment.

### Cube
A cube class has been created to enable the quick creation of textured cubes at different sizes and positions, this has been used for the buildings and vending machines.

### QuadMesh
This is the provided QuadMesh class code from A1, it has been used to create the road in the environment.

### stb_image
This open source header file has been included to facilitate the loading of texture images.

### surfaceModeller & VECTOR3D
The surfaceModeller header file has been included from A2 and the VECTOR3D header for access to the vector structs.


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
- Missile Holder
- Missile

## Model Hierarchy
```
Body
|-- Top Fin
|-- Right Fin
|-- Left Fin
|-- Command Center (Cabin)
|   |-- Missile Holder
|   |  `-- Missile
|   |-- Drive Shaft
|   |   `-- Propeller Blades
|   `-- Light Base
|       |-- Left Light
|       `-- Right Light
`-- Body Drive Shaft
    `-- Body Propeller Blades

```
