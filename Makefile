CXX = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g -I.

TARGET = webserv
SRCS = client.cpp	      HttpReq.cpp	 responseGeneration.cpp  stateMachineHandlers.cpp \
conf_parser.cpp       httpResponse.cpp	 routing.cpp		 utils.cpp \
conf_parser_util.cpp  listen_socket.cpp  routing_methods.cpp	 webserv.cpp \
httpParser.cpp	      main.cpp		 server.cpp
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
