# air-traffic-control
Final Project for COMP4900 - Group 22


Current Process:

- controlTower connects to the server
- control tower creates 10 airplane objects
  - these are structs that can be found in controlTower.h
  - a linked list is used to store these. Linked list implementation is in controlTower.c and functions in controlTower.h
- controlTower then creates three threads
  1. a plane update thread
      this thread goes through the existing linked list and updates the x,y of the airplanes with the speed of the plane
      if it sees that y goes below 0 or x goes above a MAXX, the thread broadcasts a conditional triggering the plane assignment thread
  2. plane assignment thread
      this thread is dormant until we obtain the broadcast for a plane landing. Here we go through the linked list and obtain the plane about to land. From there, we remove it from the LL using deletePlane. We then send the plane to the server - which reassembles it on the server side and calculates a checksum for integrity verification
  3. planeCollisionCheck thread
    this thread is meant to be triggered when planes are about to collide, thereby having the same x,y tuple value. We alter the speed to prevent the collision to ensure one of the variables is diffirent. 
    
    


