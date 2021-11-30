3D Packing Genetic Algorithm - Delivery Truck
----------------------------------------------
1. Execute the application using the executable file "delivery_truck.exe"
2. Camera angles can be changed by holding any of the number keys 1 through 5.

**Note that due to the lack of multi-threading, the frame rate is tied to the speed of the genetic algorithm, 
therefore it may be necessary to hold any of the number keys for a prolonged period of time before they take effect.

- The algorithm is coded to run the genetic algorithm for 100 generations at a time and then restart with the same package list with a different population.
- The parameters of the algorithm can be changed using the "#define"s in the DeliveryTruck.cpp file.
- All relevant code to the genetic algorithm will be in "DeliveryTruck.cpp", the rest of the code in Engine and main.cpp are for the graphics engine I wrote in OpenGL.
