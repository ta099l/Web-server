CXX = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g -I.

TARGET = webserv
SRCS = main.cpp conf_parser.cpp conf_parser_util.cpp webserv.cpp server.cpp utils.cpp socket/listen_socket.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	find . -name "*.o" -delete

fclean: clean
	rm -f $(TARGET)

re: fclean all
