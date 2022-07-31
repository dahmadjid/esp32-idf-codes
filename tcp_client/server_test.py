import socket
import time

#set up of server
server_address = ("", 6205)
# "" stands for the default ip of the server (its not automatically chosen apparently)
# 6205 is the port the server will open on
# you can find your ip address by open cmd prompt on windows and running "ipconfig" command
# you will find it called IPv4
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
# here we are creating our socket object 
# af_inet is saying that we are using IPv4 adresses type and not IPv6 type
# sock_stream is saying we are using TCP protocol (there is other types such as UDP)

server_socket.bind(server_address)  
#here we bind the socket we created to ip and port of the device we have chosen

server_socket.listen(4) 
# here the server is listening for upcoming connections on port 6205

client_socket, client_address = server_socket.accept() 
print("connection from :", client_address)
# this is the accepting function. 
# this line wont excute until there is upcoming connection
# it will return a socket object that we can use to send and recive data. 
# (we dont use the server socket because there can be multiple connections at the same time)
# it will also return client_address which is just a tuple (ip address, port)


data = client_socket.recv(1024).decode("utf-8")  
print(data)
# 1024 is buffer size. this should be as large as we expect the data to be. 
#.decode('utf-8') is decoding bytes into a readable string

time.sleep(1)   # 1 sec delay

message = b'hello from server' # the b here before string would encode the string into bytes 
client_socket.send(message) # message sent to the client