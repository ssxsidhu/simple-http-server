#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define PORT 8080
#define WEB_ROOT "./public"

int serve_file(int socket_fd, char* file_path) {
    // Open the file
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1) {
        return 404;
    }

    // Get the file size
    struct stat file_stat;
    if (fstat(file_fd, &file_stat) == -1) {
        close(file_fd);
        return 500;
    }

    // Read the file contents
    char* file_contents = (char*)malloc(file_stat.st_size);
    ssize_t bytes_read = read(file_fd, file_contents, file_stat.st_size);
    close(file_fd);
    if (bytes_read != file_stat.st_size) {
        free(file_contents);
        return 500;
    }

    // Send the response headers and body
    char response_headers[1024];
    snprintf(response_headers, 1024, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n", bytes_read);

    write(socket_fd, response_headers, strlen(response_headers));
    write(socket_fd, file_contents, bytes_read);

    free(file_contents);
    return 200;
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        char buffer[1024] = {0};
        valread = read(new_socket, buffer, 1024);

        printf("%s\n", buffer);

        char request_method[16], request_path[1024], request_protocol[16];
        sscanf(buffer, "%s %s %s", request_method, request_path, request_protocol);

        if (strcmp(request_method, "GET") == 0) {
            char file_path[1024];
            snprintf(file_path, 1024, "%s%s", WEB_ROOT, request_path);

            if (request_path[strlen(request_path) - 1] == '/') {
            snprintf(file_path, 1024, "%s%sindex.html", WEB_ROOT, request_path);
            int status_code = serve_file(new_socket, file_path);
            if (status_code == 200) {
                // File served successfully
                continue;
            }
        }

            int status_code = serve_file(new_socket, file_path);
            if (status_code != 200) {
                char* error_message;
                if (status_code == 404) {
                    error_message = "404 Not Found";
                } else {
                    error_message = "500 Internal Server Error";
                }

                char error_response[1024];
                snprintf(error_response, 1024, "HTTP/1.1 %s\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n%s", error_message, strlen(error_message), error_message);

                            write(new_socket, error_response, strlen(error_response));
        }
    } else {
        // Unsupported request method
        char response_headers[1024];
        snprintf(response_headers, 1024, "HTTP/1.1 405 Method Not Allowed\r\nAllow: GET\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n");
        write(new_socket, response_headers, strlen(response_headers));
    }

    close(new_socket);
}

return 0;
}
