# STM32 F3 Discovery compiler for Linux
Artem Bordiuh, Gabriel Yano. Conestoga
**********************************************************************
This project is a simple monitor program for the STM32F303VCT6 on the
STM32F3 Discovery board.  This processor has 256K of flash, and 48K of
RAM. The Discovery board has 10 LEDs, a 3 axis Gyroscope, 3 axis
accelerometer, and a 3 axis magnetic sensor.

The Simple monitor program provides a command line parser, with
facilities for the user to add new commands on a module by module
basis.  The parser is capable of recognizing one word commands, and
calling a function that implements that command.  The function may
then retrieve additional arguments from the command line, and act
accordingly.  Online brief help is provided automatically, extended
help is available optionally on a command by command basis.

The structure of the simple monitor program is one of a
foreground/background system.  The main loop of the program
initializes all the peripherals, enables interrupts, sets up I/O
conditions, and then calls a series of 'Tasks' in sequence.  The user
can extend this by adding their own background tasks to the main loop.
Caution should be exercised by the user to ensure that every task
returns back to the main loop if there is no work to do. Any task that
blocks waiting for an I/O event to occur will stall the whole
system. (For example, calling the delay() function inside your task is
frowned upon, use a timer or virtual timer instead.)

Note: There are multiple STM32F3Discovery board versions out there.
The Rev B. boards come preloaded with STLINK-V2, whereas the Rev
C. boards have STLINK-V2-1.  OpenOCD cannot automatically determine
the difference.  This means that in the Makefile, if you have a Rev
B. board, you will have to uncomment the relevant lines before being
able to program it.  The default is Rev C now.

# Installation
Open an Ubuntu terminal window by pressing CTRL-ALT-T
1. From the command prompt execute the following:
sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
sudo apt-get update
sudo apt-get install gcc-arm-none-eabi

2. Install:
sudo apt-get install libftdi-dev libusb-1.0-0-dev

3. You now need to install the On Chip Debugger support. You should be 
able to find the file in the tools folder. Download it directly into 
Ubuntu and expand the file into the directory structure which will 
contain a directory called openocd-0.9.0. Go into the directory you 
expanded it to and type:
./configure

4. Then in this directory type "make"

5. Then in this directory type "sudo make install"

6. You can test if the open ocd install worked by running the following command:
/usr/local/bin/openocd --version

7. To enable st-link support you might have to run the openocd command:
sudo ./configure --enable-stlink

8. To enstall minicom perform the following:
sudo apt-get install minicom

9. To run minicom with your USB cables connected to the STM32 board run (rev b boards it is
ttyACM0):
sudo minicom -D /dev/ttyACM1

The following steps if done correctly will make it so you do not have to put in sudo all the time for the
make commands and minicom.

1. Minicom accessing the serial port. Initial default configuration of Minicom must be done as root
still though. The user that they are using must be in the 'dialout' group (loginname is the name
you used when you created the ubuntu install):
usermod -a -G dialout loginname2. Programming the board. Two things need to happen, the user must be in the 'plugdev' group,
and the permissions on the USB device must be set correctly. The udev daemon is responsible
for setting the file permissions, and in order to to it correctly, a config file must be loaded into
the udev config directory. I'm not certain of the name for the file, but it's the only '.rules' file in
the contrib directory. Again loginname is the user name you created the ubuntu install with.
usermod -a -G plugdev loginname
cp /usr/local/share/openocd/contrib/99-openocd.rules /etc/udev/rules.d/

2. Avoiding the '~~~x~~~x~~' and the ttyACM0 port being busy for 30 sec after attaching. This
happens because of a daemon called 'modemmanager' attempting to configure the virtual com
port as a modem. If we remove the package, we get rid of the problem.
apt-get remove modemmanager

3. After all of those steps, the user should log out and log back in (to get their userid properly into
both groups), and they should completely unplug and plug back in the STM32F3Discovery if they
had it plugged in before. (To refresh the file permissions on the USB device) A single 'sudo make
clean' might need to be issued in the build directory, if they had previously compiled the code
with sudo.
