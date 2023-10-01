# TCP-Client-Server

This project provides a simple demonstration of a TCP Client-Server communication where the client sends a message to the server and receives an acknowledgment.

Communication Protocol
Protocol Definition:
Connection Request:

The client sends a connection request with its unique client ID.
Frame Format: C:<client_id>:
Message Sending:

The client can send a message (up to 100 characters) to the server.
Frame Format: M:<client_id>:<message_content>
Message Acknowledgment:

The server acknowledges the receipt of the message by appending [ACK:<client_id>] to the received message and sending it back to the client.
Disconnection:

The client sends a disconnection request.
Frame Format: D:<client_id>:
Frame Example:
Connection Request: C:1234:
Message Sending: M:1234:Hello, Server!
Server's Acknowledgment: Hello, Server! [ACK:1234]
Disconnection: D:1234:
TCP Server:
Listening Address: 127.0.0.1
Port: 1502
The server can handle communications from multiple clients simultaneously.
TCP Client GUI:
The GUI contains the following elements:

Text Edit Areas:

Server Address: Enter the server's address for the client to connect to.
Client ID: Specify the ID of the current client.
Text to Send: Input the text that will be sent to the server.
Push Buttons:

Connect Button: Set up the TCP connection between the client and server.
Disconnect Button: Disconnect from the server.
Send Button: Send the entered message to the server.
Text Browsers:

System Logs: Displays connection status changes and history of messages sent to the server.
Received Messages: Shows the history of messages received from the server.
How to Run:
Start the server application.
Start one or multiple client applications.
Use the client GUI to send messages and monitor the communication.
