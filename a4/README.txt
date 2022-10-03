Implemented a gateway controller that is responsible for reading temperature and humidity sensors then adjusting the AC system accordingly.

Gateway Controller: gateway.c
The gateway controller is mainly responsible for reading temperature and humidity sensors,  and adjusting the AC system accordingly. The AC system has two components, the chiller (for cooling) and the dehumidifier. The gateway controller manages each of them independently.


Humidity and Temperature Sensors:
The two sensors simulate the behaviour of a real sensor that reads the environment and reports the changes at a pre-defined frequency.
The sensors in this assignment do the following tasks:

1) Handshake with the gateway controller. This process will register a device with the gateway.
2) Generate temperature and humidity levels based on the AC condition reports. 
3) Create and maintain a TCP socket communication with the gateway controller. 

Run gateway <port number> in one terminal and run one or more temperature and or humidity programs in additional terminal windows.