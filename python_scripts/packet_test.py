# This script is for sending out test packets that simulate the response from the
# Strom32 gimbal control module
#
# Nik Clark
# 1/30/2021

import serial.tools.list_ports
import serial

def debug_print_serial_port_data():
	comlist = serial.tools.list_ports.comports()
	connected = []
	for element in comlist:
		connected.append(element.name)
		print("device: {}".format(element.device))
		print("name: {}".format(element.name))
		print("description: {}".format(element.description))
		print("hwid: {}".format(element.hwid))
		print("vid: {}".format(element.vid))
		print("pid: {}".format(element.pid))
		print("serial number: {}".format(element.serial_number))
		print("location: {}".format(element.location))
		print("manufacturer: {}".format(element.manufacturer))
		print("product: {}".format(element.product))
		print("interface: {}".format(element.interface))
		print("\n\n")

		
def send_packet(uart):
	pkt_size = 5
	packet = bytearray(pkt_size)

	packet[0] = 0xFA	#start
	packet[1] = 0x00	#size
	packet[2] = 0x01	#command
	packet[3] = 0x31	#checksum-low
	packet[4] = 0xE1	#checksum-high
	
	#checksum can be caluclated at crccalc.com using CRC-16/MCRF4XX

	cnt = uart.write(packet)
	
	print("{}/{} bytes sent".format(cnt, pkt_size))


def recv_packet(uart):
	
	# read start byte
	start_byte = 0x1;
	start_byte = uart.read(1)
	if start_byte == 0:
		print("No packet recieved")
		return 0
	
	# next byte is the packet length
	pkt_length = uart.read(1)
	pkt_length = int(pkt_length.encode('hex'),16)
	
	rtn_pkt = bytearray(pkt_length + 5)
	
	rtn_pkt[0] = start_byte
	rtn_pkt[1] = pkt_length
	rtn_pkt[2] = uart.read(1)
	
	#read remaining bytes
	for x in range(pkt_length+2):
		rtn_pkt[x+3] = uart.read(1)
	
	return rtn_pkt

def main():
	comlist = serial.tools.list_ports.comports()
	port_found = False
	basys_port = ""
	for dev in comlist:
		if (dev.manufacturer == "Digilent"):
			basys_port = dev.device
			port_found = True
	
	if port_found:
		print("\n\nbasys3 device found at port: {}".format(basys_port))
		
		uart = serial.Serial(basys_port)
		uart.baudrate = 115200
		uart.timeout = 2.5

		send_packet(uart)
		rtn_pkt = recv_packet(uart)
				
		if(rtn_pkt != 0):
			print("\nPacket recieved: ")
			for x in rtn_pkt:
				print( hex(x) )

		uart.close()
	else:
		print("Error: No basys3 device found")






if __name__ == "__main__":
	main()
