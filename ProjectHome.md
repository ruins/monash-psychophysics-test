### Code Repository ###
For Tools to assist in Experimental Designs to be used in the field of Psychophysics particularly the Vision Sense

### The Aim ###
This Project is to provide implementations for the purpose of providing Psychophysics Tools to collect data to be tied in with the Monash Bionic Eye

## Equipment ##
-DE2 Board provided by Terasic, designed by Altera

-SD card <= 2GB, Make sure formatted to FAT16, it can only reference from ID's 4000+ to 65535(2^16), so make sure your allocation size is within that boundary when you format it

-Altera Quartus, SOPC, NIOS-II Eclipse IDE and Matlab software

-Sensor Circuit (Optional) for latency test

# The Experiment #
Flashes images at subjects and asks them to determine whether he/she thinks it is a human face or and non-face

## Hardware and Software features ##
-Hardware counters and segment 7 displays for accurate measurements,

-Latency test of processing image to output VGA, feedback input from expansion port

-Elasped Time of program running from any point A to any point B within the program

-Keylogging Details written back to sd card

-System is controlled by a real-time kernal uC/OS-II

## GUI and Image Processing ##
Provide GUI for Experimentor to setup the parameters and to additionally generate a random set of images from a big pool of image sets.

Process Images to Model what a person would roughly be percieving from the stimulated images to the brain. By doing a gaussian distribution on NxN blocks of pixels

## Archive ##
Split into two sections;
> High Level Implementations:
> > C ( GCC Compiler ) - file conversion,
> > Matlab - image processing, file conversion
> > Matlab Toolbox ( GUI ) - Experimentor User Interface

> Low Level Implementations:
> > Verilog HDL - Hardware Comunications, DE2 FPGA Altera, Quartus
> > SOPC Builder - IP core implementations
> > NIOS-II (Eclipse IDE) - Comunication with softcore microprocessor
> > C ( Eclipse IDE ) - High level implementation on NIOS-II MP

If there are any questions or bug reports,

> please contact
> > sheejia.chin@gmail.com
> > or dream.sigil@gmail.com