Overview:

This document describes the requirements and use case of the India chat application for our group. Project India  provides an application that allows users to connect to a chat room and exchange messages with other users . We have chosen to develop this chat application to provide a way for people that are tied to their phones to use computer to communicate .Therefore this will provide  users additional way to have a real-time communication in a busy office environment where the use of mobile chat applications during office hours may be otherwise prohibited.
India Chat application is a basic and simple application that is developed and intended to be running on a computer terminal. At this early stages and with respect to the scope of the resources at hand, this application will be available and supported to run on the Linux environment . However if we have time and enough resources, we will make it to run on all the different operating systems. The chat program is written in C . Our main goal is to make the application user friendly, robust and efficient .

####Vision :

With regards to the project requirements, We have decided that this chatting application will offer functions similar to the functionalities provided by the IRC ,therefore the India chat Application will  facilitate communication in the form of pure text and could later be upgraded to newer versions with additional graphical user interface enhancements .

The Chat application will allow user  to participate in a live chatroom as well as private chats with other members. It provides a mechanism for multiple users, group of users (channel) to send and receive messages from each other, display and update the last exchanged messages so that the chat users can follow the ongoing conversations.Therefore the application would support and allow for multiple users to participate in a one-to-many as well as  one-to-one communication.

####System/Application Overview:

As stated  above the chat application has two types of clients (users) that include private clients (private users) and channel clients (group users). Therefore the chat application is separated into chat rooms and one-to-one private chat communication as  shown in the figure below. For the moment the plan is that it can be used internally on a localhost and between multiple machines connected over the Internet.  For the time being, users can join and exit the chat room at free will without any restrictions and new chat rooms can be created on the fly to suit users needs. All messages are sent and received in the text format and are not encrypted. It is our hope that future updates will add more functionalities such as data encryption, file upload and download, chat history search and many more. For now, we would like to keep the application simple ,robust ,reliable and scalable.

###Figure 1 - a sample architecture
![System architecture](/india/src/image/India_architecture.png?raw=true "system architecture")
###Figure 2 - Communication protocol of App
![System architecture](/india/src/image/indiasystem.png?raw=true "Communication protocol")
### Instructions
Building Apps
In order to use the India chat the user shall first build client and server applications.
Software can be downloaded by using following command:


Clone the repository first
#Server
After SW is downloaded the server shall be set-up up first.
Follow the following steps:
Go to the server folder of the downloaded india SW and run the make:
Folder: \india\src\server
Command:make
After the make has created the server app it can be started with: ./server command in command terminal. Then the server will ask TCP port number for listening incoming connections.
Server will also print available IP addresses for user to be used with client. After this the server will go to the daemon mode and used terminal can be closed.    

#Client
After the server is up and running it is time to make the client app.
Go to the client folder of the downloaded india SW and run the make:
Folder: \india\src\client
Command:make
After the make has created the client app it can be started with: ./client <server IP address> <port number> command in command terminal. Then the client app will open and first show the connection status with server and give choices for registration or login.
More instructions will be shown to the user after that.
