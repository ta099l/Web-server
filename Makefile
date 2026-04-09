CXX = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g -I.

TARGET = webserv
OBJ_DIR = obj

SRCS = client.cpp HttpReq.cpp responseGeneration.cpp stateMachineHandlers.cpp \
       conf_parser.cpp httpResponse.cpp routing.cpp utils.cpp \
       conf_parser_util.cpp listen_socket.cpp routing_methods.cpp webserv.cpp \
       httpParser.cpp main.cpp server.cpp

# This transforms 'main.cpp' into 'obj/main.o'
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(TARGET)

# Rule to compile .cpp files into .o files inside the OBJ_DIR
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CFLAGS) -c $< -o $@

# Create the directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
