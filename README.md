# ICMPDumper
This code creates a raw socket, crafts an IP header with a custom ICMP payload, and sends it to a target IP address with a spoofed source IP address. It uses sendto to transmit the packet.

ICMPDumper Setup Instructions

This repository contains a tool called ICMPDumper written in C. It is used for crafting and sending raw ICMP packets over the network. The main script in this repository is icmpdumper.c.

Steps to set up the repository:

1. Clone the repository
   First, you need to clone the repository to your local machine using the following command:
   git clone https://github.com/SleepTheGod/ICMPDumper.git
   Navigate to the repository folder:
   cd ICMPDumper

2. Install required dependencies
   ICMPDumper requires build tools and networking libraries. On a Linux system, you can install them by running:
   sudo apt update
   sudo apt install -y build-essential net-tools

3. Compile the source code
   To compile the ICMPDumper script, use the following command:
   gcc -o icmpdumper icmpdumper.c
   This will generate an executable called icmpdumper in the current directory.

4. Running ICMPDumper
   You can now run the ICMPDumper tool using the following command:
   sudo ./icmpdumper <ip_spoof> <dest_ip>
   Replace <ip_spoof> with the source IP you want to spoof and <dest_ip> with the destination IP address you want to target.

5. Important notes
   - Ensure you have the necessary permissions to send raw packets on your system (usually requires root).
   - Make sure the IP addresses you use are valid and properly configured in your network setup.

License
This project is licensed under the MIT License - see the LICENSE file for details.
