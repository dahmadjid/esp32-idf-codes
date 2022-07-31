import socket
import time

now = time.time()
server_address = ("192.168.0.34", 6205) 

client_socket1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
client_socket1.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
client_socket1.settimeout(3)
client_socket1.connect(server_address) 

client_socket2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
client_socket2.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
client_socket2.settimeout(3)
client_socket2.connect(server_address) 

client_socket3 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
client_socket3.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
client_socket3.settimeout(3)
client_socket3.connect(server_address) 


print("connected")

message1 = b'Hi from Client 1'
message2 = b'Hi from Client 2'
message3 = b'Hi from Client 3'

client_socket1.send(message1) 
client_socket2.send(message2) 
client_socket3.send(message3) 


data1 = client_socket1.recv(1024) 
data2 = client_socket2.recv(1024) 
data3 = client_socket3.recv(1024) 

print("message recieved", data1, len(data1))
print("message recieved", data2, len(data2))
print("message recieved", data3, len(data3))

client_socket1.close()
client_socket2.close()
client_socket3.close()

print(time.time() - now)
