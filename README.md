# HearUs
Announcement device application and client for DM&amp;P's Mity-Mite Module

## Description

This is a project from our lab at university (around 2005) that we build an announcement system for our management to use. 

Our lecturer bought [DM&amp;P's Mity-Mite Module](http://www.dmp.com.tw/app/mitymite/), so we want to build someting with it. It had a build-in x86 based  all-in-one computer (386SX @66Mhz) chip and RTL8019 network chip. 

![DM&amp;P's Mity-Mite Module](mm.jpg?raw=true "DM&amp;P's Mity-Mite Module")

I create two projects for this module. The first one is in HearUsDevice folder and a server application which runs on DOS and build with Turbo C compiler. This application runs on boot, changes display into VGA mode, opens sockets and waits for announcements to arrive and display.

The second project was build on Visual C++ (MFC) and its in HearUsClient folder. This project connects and sends the announcement texts to the device.


## License

This project is licensed under the GNU General Public License - see the LICENSE file for details