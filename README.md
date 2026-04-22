*This project has been created as part of the 42 curriculum by <login1>, <login2>.*

# Webserv

## Description
**Webserv** is a custom HTTP server built from scratch in C++, inspired by real-world web servers such as Nginx.  
The goal of this project is to understand the fundamentals of how web servers work, including handling HTTP requests, managing connections, and serving static and dynamic content.

The server is capable of:
- Handling multiple client connections using non-blocking I/O
- Processing HTTP requests (GET, POST, DELETE)
- Serving static files
- Executing CGI scripts
- Managing configuration through a custom config file

This project focuses heavily on system programming concepts such as sockets, file descriptors, multiplexing (e.g., `epoll`), and process management.

---

## Instructions

### Compilation
Clone the repository and compile using `make`:

```bash
git clone <repository_url>
cd webserv
make
```

### Execution
Run the server with a configuration file:

```bash
./webserv conf/config.conf
```

### Notes
- Ensure the configuration file path is correct
- Default port and routes are defined inside the config file
- You can test the server using a browser or tools like `curl`:

```bash
curl http://127.0.0.1:8080
```

---

## Features
- Non-blocking server using `epoll`
- HTTP/1.1 request parsing
- Static file serving
- CGI execution support
- Configurable routes and ports
- Error handling with custom error pages

---

## Usage Example

```bash
# Start server
./webserv conf/config.conf

# Send request
curl -X GET http://localhost:8080/index.html
```

---

## Technical Choices
- Language: C++
- Multiplexing: `epoll`
- Process handling: `fork` for CGI execution
- Architecture: Event-driven server model
- Configuration: Custom `.conf` parser inspired by Nginx

---

## Resources

### Documentation & References
- RFC 7230 - Hypertext Transfer Protocol (HTTP/1.1)
- Beej’s Guide to Network Programming  
  https://beej.us/guide/bgnet/
- Linux `epoll` documentation  
  https://man7.org/linux/man-pages/man7/epoll.7.html
- Nginx documentation  
  https://nginx.org/en/docs/

### Tutorials & Articles
- HTTP protocol explained (MDN)  
  https://developer.mozilla.org/en-US/docs/Web/HTTP
- Socket programming in C/C++
- CGI basics and execution flow
- https://nginx.org/en/docs/beginners_guide.html
- https://medium.com/@avocadi/what-is-epoll-9bbc74272f7c

### AI Usage
AI tools (such as ChatGPT) were used during this project for:
- Understanding HTTP protocol behavior and edge cases
- Debugging complex issues related to sockets and `epoll`
- Clarifying concepts such as CGI execution and process handling
- Reviewing and improving code structure and readability

All core implementation, design decisions, and debugging were performed manually by the project authors.

---

## Authors
- tabuayya
- balhamad
- rabusala
