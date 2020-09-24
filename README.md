# Project-1---CS-3516-Client-Server-Project
Created a TCP client/server program using socket programming

You can input necessary commands into client or server to get it to work
To retrieve HTML file for a webpage use commands such as:
"./client www.google.com 80" will work for example for web server

For local server open up a port using
"./server <Port Number>"
"./client [LOCAL HOST SERVER URL] <Filename> <Port Number>" to retrieve index.html file from server
  
The client also takes commands like
"./client -p [WEB SERVER URL] [PORT NUMBER]" To print the round trip time for any given connection

Makefile doesn't work it kept breaking
Use rm -f client or rm -f server as necessary

I didn't create executables so you could do so simply by entering the following commands:
gcc -o client client.c\
gcc -o server server.c}
