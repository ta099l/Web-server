CXX = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

TARGET = webserv
SRCS = main.cpp conf_parser.cpp conf_parser_util.cpp webserv.cpp server.cpp utils.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all
