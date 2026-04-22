*This project has been created as part of the 42 curriculum by tabuayya, balhamad, rabusala.*

# Webserv

## Description

**Webserv** is a custom HTTP/1.1 server built from scratch in C++, inspired by real-world web servers such as Nginx. The goal of this project is to deeply understand the fundamentals of how web servers operate — including parsing HTTP requests, managing concurrent connections, and serving both static and dynamic content.

The server is capable of:
- Handling multiple client connections simultaneously using non-blocking I/O
- Processing HTTP methods: `GET`, `POST`, and `DELETE`
- Serving static files from configurable root directories
- Executing CGI scripts (e.g., Python, Bash)
- Managing server behavior through a custom Nginx-inspired configuration file

This project focuses heavily on low-level system programming: POSIX sockets, file descriptor management, I/O multiplexing via `epoll`, and child process management with `fork`.

---

## Instructions

### Compilation

Clone the repository and compile using `make`:

```bash
git clone git@github.com:ta099l/Web-server.git webserv
cd webserv
make
```

### Execution

Run the server by providing a configuration file:

```bash
./webserv conf/config.conf
```

### Testing

You can test the server using a browser or tools like `curl`:

```bash
curl http://127.0.0.1:8080
```

### Notes

- Ensure the configuration file path is correct before starting the server.
- Default port and route definitions are declared inside the config file.
- CGI scripts must be placed in the configured `cgi-bin` directory and have execute permissions.

---

## Features

- Non-blocking event-driven server using Linux `epoll`
- Full HTTP/1.1 request parsing (method, headers, body)
- Static file serving with MIME type detection
- CGI script execution (`fork` + `execve`) for dynamic content
- Configurable virtual hosts, ports, routes, and error pages
- Custom error handling with user-defined error pages
- Support for chunked transfer encoding

---

## Usage Examples

### Start the Server

```bash
./webserv conf/config.conf
```

---

### GET Request

Fetch a static file from the server:

```bash
curl -X GET http://127.0.0.1:8080/index.html
```

---

### POST Request

Send form data to the server:

```bash
curl -X POST http://127.0.0.1:8080/form -H "Content-Type: application/x-www-form-urlencoded" \
  -d "name=tasnim&age=21"
```

---

### DELETE Request

Remove a resource from the server:

```bash
curl -X DELETE http://127.0.0.1:8080/file.txt
```

---

### CGI Execution

#### GET request with query parameters

```bash
curl "http://127.0.0.1:8080/cgi-bin/script.py?name=tasnim&age=21"
```

#### POST request to a CGI script

```bash
curl -X POST http://127.0.0.1:8080/cgi-bin/script.py -H "Content-Type: application/x-www-form-urlencoded" -d "name=tasnim&age=21"
```

> **Note:** Replace `script.py` with your actual CGI script filename. Ensure CGI is enabled in your config file and that the `/cgi-bin/` route is properly defined.

---

## Technical Choices

| Area | Choice | Rationale |
|---|---|---|
| Language | C++ | Required by subject; allows fine-grained memory and system control |
| I/O Multiplexing | `epoll` | Efficient event notification for large numbers of file descriptors on Linux |
| Process Handling | `fork` + `execve` | Standard UNIX model for CGI subprocess isolation |
| Architecture | Event-driven (reactor pattern) | Handles concurrent connections without threads |
| Configuration | Custom `.conf` parser | Inspired by Nginx syntax; supports multiple server blocks and location directives |

---

## Resources

### Specifications & Documentation

- [RFC 7230 — HTTP/1.1 Message Syntax and Routing](https://datatracker.ietf.org/doc/html/rfc7230)
- [Linux `epoll` man page](https://man7.org/linux/man-pages/man7/epoll.7.html)
- [Nginx Documentation](https://nginx.org/en/docs/)
- [Nginx Beginner's Guide](https://nginx.org/en/docs/beginners_guide.html)

### Tutorials & Articles

- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [HTTP — MDN Web Docs](https://developer.mozilla.org/en-US/docs/Web/HTTP)
- [What is epoll? — Medium](https://medium.com/@avocadi/what-is-epoll-9bbc74272f7c)
- Socket programming in C/C++ (various sources)
- CGI specification and execution flow (RFC 3875)

### AI Usage

AI tools (ChatGPT, Claude) were used during this project for the following purposes:

- **Understanding protocol behavior**: Clarifying edge cases in HTTP/1.1 (chunked encoding, keep-alive, header folding)
- **Debugging**: Diagnosing subtle bugs related to `epoll` event flags and non-blocking socket behavior
- **Concept clarification**: Understanding CGI execution flow, environment variable passing, and `fork`/`execve` interaction
- **Code review**: Getting feedback on code structure, readability, and potential memory leaks

All core implementation decisions, architecture design, and manual debugging were performed by the project authors. AI was used strictly as a reference and sounding board — not as a code generator.

---

## Authors

- [tabuayya](https://github.com/ta099l)
- [balhamad](https://github.com/balhamad)
- [rabusala](https://github.com/ran244)
