# Simple HTTP Server in C
This is a simple HTTP server written in C that can serve static files from a web root directory. The server listens on port 8080 and responds to GET requests for files in the web root directory.

## Requirements
To compile and run the server, you'll need:

- A C compiler (such as gcc)
- A Unix-like operating system (such as Linux or macOS)

## Usage
Clone the repository:
```
git clone https://github.com/ssxsidhu/simple-http-server.git
```

Change into the simple-http-server directory:

```
cd simple-http-server
```
Compile the server:

```
gcc server.c -o server
```
Start the server:
```
./server
```
This will start the server and listen on port 8080 for incoming requests.

In a web browser, navigate to http://localhost:8080/ to view the contents of the web root directory. You can also request individual files by specifying their paths, such as http://localhost:8080/index.html.

## Customization
To customize the server, you can modify the following variables in the server.c file:

- WEB_ROOT: The path to the directory containing the files to be served by the server. By default, this is set to "./public".
- PORT: The port number on which the server should listen for incoming requests. By default, this is set to 8080.